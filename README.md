# LVGL Color Squares for ESP32-S3 with RGB Display (ST7701) and Touch (GT911)

Проект демонстрирует работу графической библиотеки **LVGL 9** на плате **ESP32-S3** с RGB-дисплеем **800×480** (контроллер **ST7701**) и сенсорным экраном **GT911**.

## 📸 Функциональность

- **Стартовый экран**:
  - Чёрный фон.
  - Синяя рамка с отступом 2 пикселя от краёв, толщина 2 пикселя.
  - Три цветных квадрата 150×150 (красный, зелёный, синий).
  - Подписи под квадратами: Red, Green, Blue.

- **Полноэкранный режим**:
  - При касании квадрата экран заливается соответствующим цветом.
  - В центре отображается название цвета крупным шрифтом.

- **Возврат на стартовый экран**:
  - Касание в любом месте полноэкранного режима возвращает стартовый экран.

- **Обработка сенсора**:
  - Ручной опрос GT911 через I2C (без использования LVGL indev).
  - Защита от дребезга (однократное срабатывание на касание).

## 🛠️ Используемые технологии и компоненты

- **ESP-IDF** v6.1 (ветка master, совместимая с ESP32-S3)
- **LVGL** v9.2.0
- **esp_lvgl_port** – адаптер LVGL для ESP
- **esp_lcd_st7701** – драйвер RGB-дисплея
- **esp_lcd_touch_gt911** – драйвер сенсора GT911
- **PSRAM** (8 MB) – для кадрового буфера

## 📂 Структура проекта
lvgl_color_squares/
├── main/
│ ├── CMakeLists.txt
│ ├── main.c # точка входа
│ ├── display_init.h
│ ├── display_init.c # инициализация RGB-дисплея
│ ├── touch.h
│ ├── touch.c # инициализация GT911, опрос касаний
│ ├── ui.h
│ └── ui.c # UI (стартовый экран, полноэкранный режим)
├── CMakeLists.txt
└── sdkconfig.defaults # фиксированные настройки (PSRAM, шрифты, I2C)

text

## 🔌 Аппаратные подключения

### RGB-дисплей (ST7701, 800×480)

| Сигнал | GPIO |
|--------|------|
| HSYNC  | 46   |
| VSYNC  | 3    |
| DE     | 5    |
| PCLK   | 7    |
| DATA0  | 14   |
| DATA1  | 38   |
| DATA2  | 18   |
| DATA3  | 17   |
| DATA4  | 10   |
| DATA5  | 39   |
| DATA6  | 0    |
| DATA7  | 45   |
| DATA8  | 48   |
| DATA9  | 47   |
| DATA10 | 21   |
| DATA11 | 1    |
| DATA12 | 2    |
| DATA13 | 42   |
| DATA14 | 41   |
| DATA15 | 40   |
| BL     | 45   | (подсветка)

### Сенсор GT911 (I2C)

| Сигнал | GPIO |
|--------|------|
| SDA    | 8    |
| SCL    | 9    |

Адрес I2C: `0x14`

## ⚙️ Конфигурация (sdkconfig.defaults)

Важные настройки:

```conf
CONFIG_ESP32S3_SPIRAM_SUPPORT=y
CONFIG_SPIRAM_MODE_OCT=y          # Octal PSRAM (для Waveshare)
CONFIG_SPIRAM_USE_CAPS_ALLOC=y
CONFIG_LV_USE_FONT_MONTSERRAT_20=y
CONFIG_LV_USE_FONT_MONTSERRAT_32=y
CONFIG_LV_MEM_SIZE=65536
CONFIG_I2C_MASTER=y
🚀 Сборка и прошивка
Клонируйте репозиторий и перейдите в папку проекта.

Настройте цепочку инструментов для ESP32-S3 (если не сделано).

Выполните:

bash
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/ttyACM0 flash monitor
(Порт может отличаться: /dev/ttyACM0, /dev/ttyUSB0 и т.д.)

🎮 Использование
При запуске отображается стартовый экран.

Одно касание (быстрое) по цветному квадрату переводит устройство в полноэкранный режим с этим цветом.

Одно касание в любом месте полноэкранного режима возвращает стартовый экран.

🧪 Примечания
Проект не использует встроенный indev LVGL, опрос сенсора выполняется вручную в цикле ui_handle_touch().

Для стабильной работы используется PSRAM для кадрового буфера.

Тайминги RGB подобраны для дисплея Waveshare ESP32-S3-Touch-LCD-7. При использовании другого дисплея может потребоваться корректировка hsync_back_porch, vsync_back_porch в display_init.c.

📝 Конвертация изображения для LVGL
1. Установка зависимостей
# Активируйте окружение ESP-IDF
source ~/esp/esp-idf/export.sh

# Установите необходимые пакеты Python
pip install Pillow pypng lz4
2. Перейдите в директорию со скриптом LVGLImage.py
bash
cd ~/esp/esp-idf/tools/lvgl/scripts
3. Конвертация изображения в C-массив
bash
python3 LVGLImage.py /home/evgeny/Изображения/RGB.png \
    --ofmt C \
    --cf RGB565 \
    --output /home/evgeny/esp/project/lvgl_color_squares/main/wallpaper800400.c
4. Параметры команды
Параметр	Описание
--ofmt C	Выходной формат C-массив
--cf RGB565	Цветовой формат (16 бит на пиксель)
--output	Путь к выходному файлу
--swap-16	Порядок байт Big Endian (опционально, для ESP32)
5. Альтернативная команда с изменением порядка байт
bash
python3 LVGLImage.py /home/evgeny/Изображения/RGB.png \
    --ofmt C \
    --cf RGB565 \
    --swap-16 \
    --output /home/evgeny/esp/project/lvgl_color_squares/main/wallpaper800400.c
6. Подключение изображения в проекте
В main/CMakeLists.txt добавьте:

cmake
idf_component_register(SRCS
    ...
    "wallpaper800400.c"
    ...
)
В коде используйте:

c
// Объявление изображения
LV_IMAGE_DECLARE(wallpaper800400);

// Создание виджета изображения
lv_obj_t *img = lv_image_create(lv_scr_act());
lv_image_set_src(img, &wallpaper800400);
7. Параметры цветовых форматов
Формат	Описание	Размер на пиксель
RGB565	16-bit RGB	2 байта
RGB565A8	16-bit RGB + 8-bit альфа	3 байта
RGB888	24-bit RGB	3 байта
ARGB8888	32-bit ARGB	

# Установка и использование конвертера шрифтов LVGL
## 1. Установка Node.js и npm (если не установлены)
bash
sudo apt update
sudo apt install nodejs npm

## 2. Установка конвертера шрифтов в проект
bash
cd /home/evgeny/esp/project/lvgl_color_squares
npm install lv_font_conv@latest

## 3. Конвертация шрифта (с поддержкой кириллицы)
bash
npx lv_font_conv --bpp 4 --size 16 --format lvgl \
    --font "/home/evgeny/esp/project/отдельные элементы кода для дисплея/arial-regular.ttf" \
    -r 0x20-0x7F,0x400-0x45F,0x401,0x451 \
    --lv-font-name my_arial \
    -o main/my_arial.c

## 4. Подключение шрифта в проекте
В main/CMakeLists.txt добавьте:

cmake
idf_component_register(SRCS
    ...
    "my_arial.c"
    ...
)
## В коде используйте:

c
// Объявление шрифта
LV_FONT_DECLARE(my_arial);

// Применение к виджету
lv_obj_set_style_text_font(label, &my_arial, 0);
5. Параметры команды
Параметр	Описание
--bpp	Бит на пиксель (1,2,4,8). 4 - хорошее качество, 1 - экономия памяти
--size	Высота шрифта в пикселях
--format lvgl	Формат вывода для LVGL
--font	Путь к TTF/WOFF файлу шрифта
-r	Диапазоны символов Unicode
--lv-font-name	Имя переменной шрифта
-o	Выходной C-файл
6. Диапазоны символов для кириллицы
Диапазон	Описание
0x20-0x7F	Латинские буквы, цифры, знаки препинания
0x400-0x45F	Русская кириллица (А-я)
0x401	Буква Ё
0x451	Буква ё

📄 Лицензия
Проект распространяется под лицензией MIT.

🙏 Благодарности
Espressif за отличный фреймворк IDF.

Команда LVGL за мощную графическую библиотеку.

Waveshare за аппаратную платформу и примеры кода.

Автор: Ваше имя / ник
Дата: 2025

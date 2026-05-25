markdown
# ESP32-S3 Display Project (LVGL + Wi-Fi AP + RTC)

Проект для ESP32-S3 с сенсорным дисплеем 800x480 (RGB, драйвер ST7701), работающий под управлением ESP-IDF v6.1.  
Реализовано:  
- Фоновое изображение (конвертированное в RGB565 C-массив)  
- Графический интерфейс на LVGL v9  
- Сенсорный экран GT911 (I2C)  
- Wi-Fi точка доступа (SoftAP) с выдачей IP через DHCP  
- TCP-сервер на порту 8888 для получения команд (например, `GET_TIME`)  
- Системное время (RTC), обновляемое раз в минуту, с форматом `dd.mm.yyyy HH:MM`  
- Поддержка кириллицы через пользовательский шрифт `my_arial`  

---

## 📁 Структура проекта
main/
├── CMakeLists.txt
├── main.c
│
├── settings/
│ ├── display/ # инициализация RGB-дисплея
│ ├── touch/ # сенсор GT911
│ ├── wifi/ # SoftAP и TCP-сервер
│ └── time/ # RTC (время из компиляции + счётчик)
│
└── GUI/
├── wallpaper/ # фоновое изображение (RGB565)
├── fonts/ # шрифт my_arial (кириллица)
└── ui/ # интерфейс LVGL

text

---

## 🔧 Требования

- **ESP-IDF v6.1** (последняя на момент разработки)
- **LVGL v9** (встроен через managed component)
- **PSRAM** (обязательно, 8 МБ)
- **Дисплей**: 800x480, RGB-интерфейс, 16 бит, драйвер ST7701
- **Сенсор**: GT911 по I2C

---

## 📦 Установка и сборка

1. **Клонировать репозиторий**  
   ```bash
   git clone https://github.com/Raven078/waveshare-ESP32s3_touch_LCD7_screentest.git
   cd lvgl_color_squares
Настроить Wi-Fi (SSID и пароль точки доступа)

bash
idf.py menuconfig
Перейти в Wi-Fi SoftAP Configuration

Задать Wi-Fi SSID и Wi-Fi Password (по умолчанию ESP32_S3_Display / 12345678)

Собрать и прошить

bash
idf.py clean
idf.py build
idf.py flash monitor
После загрузки на дисплее появится фоновое изображение, панель с информацией о Wi-Fi и текущее время (статическое, обновляется раз в минуту).

📡 Управление по TCP
Подключитесь к Wi-Fi сети устройства, затем используйте netcat или любой TCP-клиент:

bash
nc 192.168.4.1 8888
Доступные команды:

Команда	Ответ
GET_TIME	TIME:2026-05-25 14:30:00
GET_TIMESTAMP	TIMESTAMP:1747921800
🖼️ Замена фонового изображения
Подготовить PNG размером 800x480.

Конвертировать в формат RGB565 (Big‑endian) с помощью онлайн-конвертера LVGL:
https://lvgl.io/tools/imageconverter

Color format: RGB565

Output format: C array

Byte order: Big endian

Заменить содержимое main/GUI/wallpaper/wallpaper800400.c сгенерированным массивом.

🔤 Работа со шрифтами

Установка инструментов для шрифтов
Для генерации собственных шрифтов (с поддержкой кириллицы) потребуется Node.js и пакет lv_font_conv.

1. Установка Node.js и npm
bash
sudo apt update
sudo apt install nodejs npm

2. Локальная установка lv_font_conv в папке проекта
bash
cd /home/evgeny/esp/project/lvgl_color_squares
npm install lv_font_conv
После этого команда будет доступна через npx:

3. Альтернативный способ – онлайн-конвертер
Если не хотите устанавливать Node.js, используйте официальный онлайн-конвертер шрифтов LVGL:

Выбрать LVGL 9.x

Загрузить TTF-файл

Указать те же диапазоны символов

Скачать готовый C-файл и поместить в main/GUI/fonts/

Для поддержки кириллицы использован шрифт my_arial, сгенерированный через lv_font_conv.
Очень важный параметр --no-compress – без него шрифт может отображаться некорректно (пропадать текст или вызывать ошибки LVGL).

bash
npx lv_font_conv --bpp 4 --size 24 --format lvgl \
    --font "/home/evgeny/esp/project/отдельные элементы кода для дисплея/arial-regular.ttf" \
    -r 0x20-0x7F,0x410-0x44F,0x401,0x451 \
    --lv-font-name my_arial24 \
    --no-compress \ 
    -o my_arial24.c
⚙️ Основные настройки в menuconfig

Flash size → 4 MB (или больше, если есть)

PSRAM → Octal Mode PSRAM, 80 MHz, выделить .bss и .data в PSRAM

LVGL → включить RGB565 и RGB565A8, размер памяти lv_malloc ≥ 128 КБ

📄 Лицензия
Проект распространяется под лицензией MIT.

📧 Контакты
Автор: Raven078
По вопросам доработки и интеграции пишите в Issues репозитория.
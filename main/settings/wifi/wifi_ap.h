#ifndef WIFI_AP_H
#define WIFI_AP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_netif.h"

/**
 * @brief Инициализация Wi-Fi в режиме точки доступа (SoftAP)
 */
void wifi_init_softap(void);

/**
 * @brief Получить IP-адрес точки доступа
 * @return esp_ip4_addr_t IP-адрес (в сетевом порядке)
 */
esp_ip4_addr_t wifi_ap_get_ip(void);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_AP_H */
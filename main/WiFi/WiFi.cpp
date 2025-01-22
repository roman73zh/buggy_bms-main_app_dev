#include "WiFi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "NVStorage.h"

#define EXAMPLE_ESP_WIFI_SSID "ASUS-Merlin"
#define EXAMPLE_ESP_WIFI_PASS "73456117"
#define EXAMPLE_ESP_MAXIMUM_RETRY 3

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char* TAG = "WiFi";

esp_netif_t* wifi_netif;

static int s_retry_num = 0;

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "WiFi Connecting");
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        esp_err_t err;
        if(NVStorage::systemParams.dhcp) {
            ESP_LOGI(TAG, "eth_event_handler dhcp ip");
            esp_netif_dhcp_status_t dhcp_status;
            ESP_ERROR_CHECK(esp_netif_dhcpc_get_status(wifi_netif, &dhcp_status));
            if (dhcp_status != ESP_NETIF_DHCP_STARTED) {
                if(ESP_OK != (err = esp_netif_dhcpc_start(wifi_netif))){
                    ESP_LOGE(TAG, "eth_event_handler Start dhcp Err: %s", esp_err_to_name(err));
                }
            }
        } else {
            ESP_LOGI(TAG, "eth_event_handler static ip");
            if(ESP_OK != (err = esp_netif_dhcpc_stop(wifi_netif))) {
                ESP_LOGE(TAG, "eth_event_handler Stop dhcp Err: %s", esp_err_to_name(err));
            }
            esp_netif_ip_info_t ip_info;
            esp_netif_set_ip4_addr(&ip_info.ip, NVStorage::systemParams.ip[0], NVStorage::systemParams.ip[1], NVStorage::systemParams.ip[2], NVStorage::systemParams.ip[3]);
            esp_netif_set_ip4_addr(&ip_info.netmask, NVStorage::systemParams.mask[0], NVStorage::systemParams.mask[1], NVStorage::systemParams.mask[2], NVStorage::systemParams.mask[3]);
            esp_netif_set_ip4_addr(&ip_info.gw, NVStorage::systemParams.gateway[0], NVStorage::systemParams.gateway[1], NVStorage::systemParams.gateway[2], NVStorage::systemParams.gateway[3]);
            if(ESP_OK != (err = esp_netif_set_ip_info(wifi_netif, &ip_info))) {
                ESP_LOGE(TAG, "eth_event_handler set ip Err: %s", esp_err_to_name(err));
            }
        }
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;

        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        const esp_netif_ip_info_t *ip_info = &event->ip_info;

        if (NVStorage::systemParams.dhcp) {
            NVStorage::systemParams.ip[0] = ip_info->ip.addr & 0xFF;
            NVStorage::systemParams.ip[1] = (ip_info->ip.addr >> 8) & 0xFF;
            NVStorage::systemParams.ip[2] = (ip_info->ip.addr >> 16) & 0xFF;
            NVStorage::systemParams.ip[3] = (ip_info->ip.addr >> 24) & 0xFF;

            NVStorage::systemParams.gateway[0] = ip_info->gw.addr & 0xFF;
            NVStorage::systemParams.gateway[1] = (ip_info->gw.addr >> 8) & 0xFF;
            NVStorage::systemParams.gateway[2] = (ip_info->gw.addr >> 16) & 0xFF;
            NVStorage::systemParams.gateway[3] = (ip_info->gw.addr >> 24) & 0xFF;

            NVStorage::systemParams.mask[0] = ip_info->netmask.addr & 0xFF;
            NVStorage::systemParams.mask[1] = (ip_info->netmask.addr >> 8) & 0xFF;
            NVStorage::systemParams.mask[2] = (ip_info->netmask.addr >> 16) & 0xFF;
            NVStorage::systemParams.mask[3] = (ip_info->netmask.addr >> 24) & 0xFF;
        }
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta() {
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_netif = esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}
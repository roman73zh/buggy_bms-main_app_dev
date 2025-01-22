#include "NVStorage.h"

static const char *TAG = "NVStorage";


void NVStorage::init(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
}

void NVStorage::loadSystemParams(){
    esp_err_t err;
    nvs_handle_t nvs_handler;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handler);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG,"Reading system params from NVS ...");

        nvs_get_i8(nvs_handler, "dhcp", (int8_t*)&systemParams.dhcp);
        nvs_get_i8(nvs_handler, "ip1", (int8_t*)&systemParams.ip[0]);
        nvs_get_i8(nvs_handler, "ip2", (int8_t*)&systemParams.ip[1]);
        nvs_get_i8(nvs_handler, "ip3", (int8_t*)&systemParams.ip[2]);
        nvs_get_i8(nvs_handler, "ip4", (int8_t*)&systemParams.ip[3]);
        nvs_get_i8(nvs_handler, "ma1", (int8_t*)&systemParams.mask[0]);
        nvs_get_i8(nvs_handler, "ma2", (int8_t*)&systemParams.mask[1]);
        nvs_get_i8(nvs_handler, "ma3", (int8_t*)&systemParams.mask[2]);
        nvs_get_i8(nvs_handler, "ma4", (int8_t*)&systemParams.mask[3]);
        nvs_get_i8(nvs_handler, "gw1", (int8_t*)&systemParams.gateway[0]);
        nvs_get_i8(nvs_handler, "gw2", (int8_t*)&systemParams.gateway[1]);
        nvs_get_i8(nvs_handler, "gw3", (int8_t*)&systemParams.gateway[2]);
        nvs_get_i8(nvs_handler, "gw4", (int8_t*)&systemParams.gateway[3]);

        nvs_close(nvs_handler);
    }
}

void NVStorage::saveSystemParams(){
    esp_err_t err;
    nvs_handle_t nvs_handler;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handler);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Saving system params to NVS ...");
        if (systemParams.dhcp == false){
            nvs_set_i8(nvs_handler, "ip1", systemParams.ip[0]);
            nvs_set_i8(nvs_handler, "ip2", systemParams.ip[1]);
            nvs_set_i8(nvs_handler, "ip3", systemParams.ip[2]);
            nvs_set_i8(nvs_handler, "ip4", systemParams.ip[3]);
            nvs_set_i8(nvs_handler, "ma1", systemParams.mask[0]);
            nvs_set_i8(nvs_handler, "ma2", systemParams.mask[1]);
            nvs_set_i8(nvs_handler, "ma3", systemParams.mask[2]);
            nvs_set_i8(nvs_handler, "ma4", systemParams.mask[3]);
            nvs_set_i8(nvs_handler, "gw1", systemParams.gateway[0]);
            nvs_set_i8(nvs_handler, "gw2", systemParams.gateway[1]);
            nvs_set_i8(nvs_handler, "gw3", systemParams.gateway[2]);
            nvs_set_i8(nvs_handler, "gw4", systemParams.gateway[3]);
        }
        nvs_set_i8(nvs_handler, "dhcp", systemParams.dhcp);

        nvs_commit(nvs_handler);
        nvs_close(nvs_handler);
    }
}

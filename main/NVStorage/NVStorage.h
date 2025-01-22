#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

class NVStorage {
public:
    static void init();

    static inline struct system_params_t {
        bool dhcp = true;
        uint8_t ip[4] = {0};
        uint8_t mask[4] = {0};
        uint8_t gateway[4] = {0};
    } systemParams {
        .dhcp = true,
        .ip = {0},
        .mask = {0},
        .gateway = {0}
    };

    static void loadSystemParams();
    static void saveSystemParams();
};
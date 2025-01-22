#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_eth.h"
#include "Ethernet.h"
#include "Webserver.h"
#include "NVStorage.h"
#include "udp-server.h"
#include "esp_timer.h"
#include "hardware.h"
#include "WiFi.h"
#include "Page.h"
#include "Header.h"
#include "InputNumber.h"

static const char* TAG = "Main";

float voltage = 310;

Header header("Блок BMS и инверторов", "Инверторы и BMS");

Page bmsPage(&header, "Батарея", "/bms.html");
Page contactorsPage(&header, "Контакторы", "/contactors.html");
Page inverterPage(&header, "Инверторы", "/inverter.html");
Page systemPage(&header, "Система", "/system.html");

Block voltageSettings(&bmsPage, "Напряжение");
Block cutoffSettings(&bmsPage, "Отсечка");

InputNumber minVoltageInput(&voltageSettings, "minVoltage", "Минимальное напряжение", InputNumber::Float, &voltage);
InputNumber diffVoltageInput(&voltageSettings, "diffVoltage", "Разница напряжений балансировки", InputNumber::Float, &voltage);
InputNumber maxVoltageInput(&voltageSettings, "maxVoltage", "Максимальное напряжение", InputNumber::Float, &voltage);
InputNumber maxCutoffVoltageInput(&cutoffSettings, "maxCutoffVoltage", "Верхнее напряжение отсечки", InputNumber::Float, &voltage);
InputNumber minCutoffVoltageInput(&cutoffSettings, "minCutoffVoltage", "Нижнее напряжение отсечки", InputNumber::Float, &voltage);
InputNumber maxCutoffTimeInput(&cutoffSettings, "maxCutoffTime", "Время отсечки перезаряд", InputNumber::Float, &voltage);
InputNumber minCutoffTimeInput(&cutoffSettings, "minCutoffTime", "Время отсечки переразряд", InputNumber::Float, &voltage);


extern "C" void app_main(void) {
    

    NVStorage::init();
    NVStorage::loadSystemParams();

    wifi_init_sta();

    Webserver::init();
    Webserver::registerPage(&bmsPage);
    Webserver::registerPage(&contactorsPage);
    Webserver::registerPage(&inverterPage);
    Webserver::registerPage(&systemPage);

    start_udp_server();

    int64_t time = esp_timer_get_time();

    std::string result = bmsPage.serialize();

    time = esp_timer_get_time() - time;

    // ESP_LOGI(TAG, "%lld - %s", time, result.c_str());
}

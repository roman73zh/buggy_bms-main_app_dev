#include "esp_check.h"
#include "esp_eth.h"
#include "esp_netif.h"
#include "driver/spi_master.h"

class Ethernet {
private:
    typedef struct {
        uint8_t spi_cs_gpio;
        uint8_t int_gpio;
        int8_t phy_reset_gpio;
        uint8_t phy_addr;
        uint8_t *mac_addr;
    } spi_eth_module_config_t;
    inline static esp_eth_handle_t eth_handle;
    static esp_err_t spi_bus_init(uint8_t pinMiso, uint8_t pinMosi, uint8_t pinSck, spi_host_device_t spiBus);
    static esp_err_t eth_init_spi(spi_eth_module_config_t *spi_eth_module_config, spi_host_device_t spiBus);
    static void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    static void got_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
public:
    static esp_err_t init(uint8_t miso, uint8_t mosi, uint8_t sck, uint8_t pinCs, uint8_t pinInt, spi_host_device_t spiBus);
    static esp_netif_t *eth_netif;
};
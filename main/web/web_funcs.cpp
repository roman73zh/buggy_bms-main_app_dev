#include "web_funcs.h"
#include "esp_log.h"
#include "NVStorage.h"

static const char *TAG = "WebFuncs";


std::string webSystemJson() {
    std::string json = "{";
    json += "\"dhcp\":\"";
    json += (NVStorage::systemParams.dhcp == true ? "checked" : "0");

    json += "\",\"ip1\":\"";
    json += std::to_string(NVStorage::systemParams.ip[0]);
    json += "\",\"ip2\":\"";
    json += std::to_string(NVStorage::systemParams.ip[1]);
    json += "\",\"ip3\":\"";
    json += std::to_string(NVStorage::systemParams.ip[2]);
    json += "\",\"ip4\":\"";
    json += std::to_string(NVStorage::systemParams.ip[3]);

    json += "\",\"ma1\":\"";
    json += std::to_string(NVStorage::systemParams.mask[0]);
    json += "\",\"ma2\":\"";
    json += std::to_string(NVStorage::systemParams.mask[1]);
    json += "\",\"ma3\":\"";
    json += std::to_string(NVStorage::systemParams.mask[2]);
    json += "\",\"ma4\":\"";
    json += std::to_string(NVStorage::systemParams.mask[3]);

    json += "\",\"gw1\":\"";
    json += std::to_string(NVStorage::systemParams.gateway[0]);
    json += "\",\"gw2\":\"";
    json += std::to_string(NVStorage::systemParams.gateway[1]);
    json += "\",\"gw3\":\"";
    json += std::to_string(NVStorage::systemParams.gateway[2]);
    json += "\",\"gw4\":\"";
    json += std::to_string(NVStorage::systemParams.gateway[3]);

    json += "\"}";
    return json;
}

void webSystemSet(httpd_req_t *req) {
    size_t buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        char*  buf;
        buf = (char*)malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[50];
            if (httpd_query_key_value(buf, "dhcp", param, sizeof(param)) == ESP_OK) {
                NVStorage::systemParams.dhcp = std::stoi(param);
                if (!NVStorage::systemParams.dhcp) {
                    if (httpd_query_key_value(buf, "ip1", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.ip[0] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ip2", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.ip[1] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ip3", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.ip[2] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ip4", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.ip[3] = std::stoi(param);
                    }

                    if (httpd_query_key_value(buf, "ma1", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.mask[0] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ma2", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.mask[1] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ma3", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.mask[2] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ma4", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.mask[3] = std::stoi(param);
                    }

                    if (httpd_query_key_value(buf, "gw1", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.gateway[0] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "gw2", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.gateway[1] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "gw3", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.gateway[2] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "gw4", param, sizeof(param)) == ESP_OK) {
                        NVStorage::systemParams.gateway[3] = std::stoi(param);
                    }
                }
            }
            NVStorage::saveSystemParams();
        }
    }
}

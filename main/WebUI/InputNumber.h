#pragma once

#include "Input.h"
#include <esp_http_server.h>

class InputNumber : public Input {
public:
    enum DataType {
        Int32 = 0,
        Float
    };
    InputNumber(Block* block, std::string id, std::string name, DataType type, void* dataPointer, float min = 0, float max = 1000, float step = 1);
    std::string serialize() override;
    void sendHttp(httpd_req_t *req) override;
private:
    float min, max, step;
    DataType type;
    void* dataPointer;
};
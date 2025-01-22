#pragma once

#include <string>
#include <list>
#include <esp_http_server.h>
#include "Block.h"


class Block;

class Input {
public:
    Input(Block* block,std::string id, std::string name);
    virtual std::string serialize() = 0;
    virtual void sendHttp(httpd_req_t *req) = 0;
protected:
    std::string id;
    std::string name;
};
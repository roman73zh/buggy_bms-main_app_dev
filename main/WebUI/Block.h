#pragma once

#include <string>
#include <list>
#include <esp_http_server.h>
#include "Input.h"
#include "Page.h"

class Page;
class Input;

class Block {
public:
    Block(Page* page, std::string blockName);
    void addInput(Input* input);
    std::string serialize();
    void sendHttp(httpd_req_t *req);
private:
    std::string blockName;
    std::list<Input*> inputs;
};
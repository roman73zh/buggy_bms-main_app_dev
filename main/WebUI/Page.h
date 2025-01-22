#pragma once

#include <string>
#include <list>
#include <esp_http_server.h>
#include "Block.h"
#include "Header.h"

class Header;
class Block;

class Page {
public:
    Page(Header* header, std::string pageName, std::string linkUrl);
    void addBlock(Block* block);
    static esp_err_t webPageHandler(httpd_req_t *req);
    std::string serialize();
    std::string getLink();
    void sendHttp(httpd_req_t *req);
    char* getUrl();
private:
    std::string pageName;
    std::string link;
    std::list<Block*> pageBlocks;
    Header* header;
    char* url;
};
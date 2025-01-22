#pragma once

#include <string>
#include <list>
#include <esp_http_server.h>
#include "Page.h"


class Page;

class Header {
public:
    Header(std::string unitName, std::string title);
    void registerPage(Page* page);
    std::string getHeader(Page* page);
    void sendHttp(httpd_req_t *req, Page* page);
private:
    std::string unitName;
    std::list<Page*> pages;
    std::string headerStatic;
};
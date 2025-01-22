#include "Header.h"
#include "HeaderParts.h"

Header::Header(std::string unitName, std::string title) {
    this->unitName = unitName;
    headerStatic.assign(P_header_1);
    headerStatic.append(title);
    headerStatic.append(P_header_2);
    headerStatic.append(unitName);
    headerStatic.append(P_header_3);
}

void Header::registerPage(Page* page) {
    pages.push_back(page);
}

std::string Header::getHeader(Page* page) {
    std::string html = headerStatic;
    for (auto const& item : pages) {
        if (item == page) {
            html.append("\n            <li class=\"active\">" + item->getLink() + "</li>");
        } else {
            html.append("\n            <li>" + item->getLink() + "</li>");
        }
    }
    html.append(P_header_4);
    return html;
}

void Header::sendHttp(httpd_req_t *req, Page* page) {
    httpd_resp_sendstr_chunk(req, headerStatic.c_str());
    for (auto const& item : pages) {
        if (item == page) {
            httpd_resp_sendstr_chunk(req, "\n            <li class=\"active\">");
            
        } else {
            httpd_resp_sendstr_chunk(req, "\n            <li>");
        }
        httpd_resp_sendstr_chunk(req, item->getLink().c_str());
        httpd_resp_sendstr_chunk(req, "</li>");
    }
    httpd_resp_sendstr_chunk(req, P_header_4);
}
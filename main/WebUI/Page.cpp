#include "Page.h"
#include "P_index.h"
#include "PageParts.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char* TAG = "Page";

Page::Page(Header* header, std::string pageName, std::string linkUrl) {
    this->pageName = pageName;
    this->header = header;
    header->registerPage(this);
    link = "<a href=\"" + linkUrl + "\">" + pageName + "</a>";
    url = strdup(linkUrl.c_str());
}

void Page::addBlock(Block* block) {
    pageBlocks.push_back(block);
}

std::string Page::serialize() {
    std::string html = header->getHeader(this);
    html.append(P_page_1);
    html.append("<h1>" + pageName + "</h1>\n      </div>");
    for (auto const& item : pageBlocks) {
        html += item->serialize();
    }
    html.append(P_page_2);
    return html;
}

std::string Page::getLink() {
    return link;
}

// esp_err_t Page::webPageHandler(httpd_req_t *req) {
//     int64_t time = esp_timer_get_time();
//     Page* currentPage = (Page*)(req->user_ctx);
//     httpd_resp_send(req, currentPage->serialize().c_str(), HTTPD_RESP_USE_STRLEN);
//     time = esp_timer_get_time() - time;

//     ESP_LOGI(TAG, "Generation + send time %lld us", time);
//     return ESP_OK;
// }

esp_err_t Page::webPageHandler(httpd_req_t *req) {
    int64_t time = esp_timer_get_time();
    Page* currentPage = (Page*)(req->user_ctx);
    currentPage->sendHttp(req);
    httpd_resp_sendstr_chunk(req, NULL);

    time = esp_timer_get_time() - time;
    ESP_LOGI(TAG, "Generation + send time %lld us", time);
    return ESP_OK;
}

char* Page::getUrl() {
    return url;
}

void Page::sendHttp(httpd_req_t *req) {
    header->sendHttp(req, this);
    httpd_resp_sendstr_chunk(req, P_page_1);
    httpd_resp_sendstr_chunk(req, ("<h1>" + pageName + "</h1>\n      </div>").c_str());
    for (auto const& item : pageBlocks) {
        item->sendHttp(req);
    }
    httpd_resp_sendstr_chunk(req, P_page_2);
}
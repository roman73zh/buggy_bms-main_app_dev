#include "Block.h"
#include "BlockParts.h"

Block::Block(Page* page, std::string blockName) {
    this->blockName = blockName;
    page->addBlock(this);
}

void Block::addInput(Input* input) {
    inputs.push_back(input);
}

std::string Block::serialize() {
    std::string html = "\n      <h3>" + blockName + "</h3>";
    html.append("\n      <br>");
    html.append("\n      <table class=\"table\">");
    html.append("\n        <tbody>");
    for (auto const& item : inputs) {
        html += item->serialize();
    }
    html.append("\n        </tbody>");
    html.append("\n      </table>");
    html.append("\n      <br>");
    return html;
}

void Block::sendHttp(httpd_req_t *req) {
    httpd_resp_sendstr_chunk(req, ("\n      <h3>" + blockName + "</h3>").c_str());
    httpd_resp_sendstr_chunk(req, P_table_begin);
    for (auto const& item : inputs) {
        item->sendHttp(req);
    }
    httpd_resp_sendstr_chunk(req, P_table_end);
}
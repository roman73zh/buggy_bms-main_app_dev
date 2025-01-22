#include "InputNumber.h"
#include <string>
#include "Staff.h"

InputNumber::InputNumber(Block* block, std::string id, std::string name, DataType type, void* dataPointer, float min, float max, float step) : Input(block, id, name) {
    this->min = min;
    this->max = max;
    this->step = step;
    this->dataPointer = dataPointer;
    this->type = type;
}

std::string InputNumber::serialize() {
    std::string value = "";
    if (type == Int32)
        value = std::to_string(*(int32_t*)dataPointer);
    else if (type == Float)
        value = floatToString(*(float*)dataPointer);

    std::string result =  "\n          <tr>\n            <td>" + name + "</td>"
    "\n            <td><input type=\"number\" id=\"" + id + "\""
    " min=" + std::to_string(min) +
    " max=" + std::to_string(max) +
    " step=" + std::to_string(step) +
    " value=" + value + "></td>\n          </tr>";

    return result;
}

void InputNumber::sendHttp(httpd_req_t *req) {
    httpd_resp_sendstr_chunk(req, serialize().c_str());
}
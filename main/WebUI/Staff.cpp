#include "Staff.h"

std::string floatToString(float value) {
    std::string str = std::to_string(value);

    if(str.find('.') != std::string::npos) {
        str = str.substr(0, str.find_last_not_of('0')+1);
        if(str.find('.') == str.size()-1) {
            str = str.substr(0, str.size()-1);
        }
    }
    return str;
}
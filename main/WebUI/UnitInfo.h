#pragma once

#include <string>

#define UNIT_NAME       "Блок BMS и инверторов"
#define FW_VER          "2.1"
#define HW_VER          "2.0"


class UnitInfo {
public:
    static std::string getJSON();
};

#include "UnitInfo.h"

std::string UnitInfo::getJSON() {
    std::string json = "{";
    json.append("\"unitName\":\"" + std::string(UNIT_NAME) + "\", ");
    json.append("\"fwVer\":\"" + std::string(FW_VER) + "\", ");
    json.append("\"hvVer\":\"" + std::string(HW_VER) + "\", ");
    json.append("\"buildDate\":\"" + std::string(__DATE__) + "\", ");
    json.append("\"buildTime\":\"" + std::string(__TIME__) + "\"");
    json.append("}");
    return json;
}
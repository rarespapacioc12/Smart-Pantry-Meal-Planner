#include "../../include/core/Types.hpp"
#include <string>
#include <stdexcept>

std::string toString(Unit unit){
    switch(unit){
        case Unit::g:
            return "g";
            break;
        case Unit::ml:
            return "ml";
            break;
        case Unit::pcs:
            return "pcs";
            break;
    }
    return "Unknown";
}

Unit parseUnit(const std::string& s){
    if (s == "g") { return Unit::g; }
    if (s == "ml") { return Unit::ml; }
    if (s == "pcs") { return Unit::pcs; }
    throw std::runtime_error("Invalid unit: " + s + "\n");
}

std::string toString(Location location){
    switch(location){
        case Location::Freezer:
            return "Freezer";
            break;
        case Location::Fridge:
            return "Fridge";
            break;
        case Location::Pantry:
            return "Pantry";
            break;
    }
    return "Unknown";
}

Location parseLocation(const std::string& s){
    if (s == "Freezer") { return Location::Freezer; }
    if (s == "Fridge") { return Location::Fridge; }
    if (s == "Pantry") { return Location::Pantry; }
    throw std::runtime_error("Invalid Location: " + s + "\n");
}

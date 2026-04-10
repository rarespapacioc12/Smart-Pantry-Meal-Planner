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

std::string toString(Priority priority){
    switch(priority){
        case Priority::Low: return "Low";
        case Priority::Normal: return "Normal";
        case Priority::Medium: return "Medium";
        case Priority::High: return "High";
        case Priority::Urgent: return "Urgent";
    }
    return "Unknown";
}

Priority parsePriority(int i){
    switch(i){
        case 1: return Priority::Low;
        case 2: return Priority::Normal;
        case 3: return Priority::Medium;
        case 4: return Priority::High;
        case 5: return Priority::Urgent;
        default: return Priority::Normal;
    }
}

std::string toString(Certification cert){
    switch(cert){
        case Certification::EUOrganic: return "EU Organic";
        case Certification::CCPB: return "CCPB";
        case Certification::Bioland: return "Bioland";
        case Certification::Demeter: return "Demeter";
    }
    return "Unknown";
}

Certification parseCertification(const std::string& s){
    if(s == "EU Organic") return Certification::EUOrganic;
    if(s == "CCPB") return Certification::CCPB;
    if(s == "Bioland") return Certification::Bioland;
    if(s == "Demeter") return Certification::Demeter;
    throw std::runtime_error("Invalid Certification: " + s + "\n");
}


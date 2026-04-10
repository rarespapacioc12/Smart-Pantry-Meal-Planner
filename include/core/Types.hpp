#pragma once
#include <string>

enum class Unit { g, ml, pcs };
enum class Location { Pantry, Fridge, Freezer};
enum class Priority { Low = 1, Normal = 2, Medium = 3, High = 4, Urgent = 5 };
enum class Certification { EUOrganic, CCPB, Bioland, Demeter };

std::string toString(Unit);
Unit parseUnit(const std::string&);
std::string toString(Location);
Location parseLocation(const std::string&);
std::string toString(Priority);
Priority parsePriority(int);
std::string toString(Certification);
Certification parseCertification(const std::string&);
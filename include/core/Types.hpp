#pragma once
#include <string>

enum class Unit { g, ml, pcs };
enum class Location { Pantry, Fridge, Freezer};

std::string toString(Unit);
Unit parseUnit(const std::string&);
std::string toString(Location);
Location parseLocation(const std::string&);
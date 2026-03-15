#pragma once
#include "./Types.hpp"
#include <string>

class Ingredient {
private:
    std::string name_;
    Unit unit_;

    static std::string normalize(std::string);

public:
    Ingredient();
    Ingredient(const std::string&, const Unit&);
    const std::string& name() const;
    Unit unit() const;
    friend std::ostream& operator<<(std::ostream&, const Ingredient&);
};
#include "../../include/core/Ingredient.hpp"
#include <string>
#include <cctype>
#include <iostream>

Ingredient::Ingredient() = default;

std::string Ingredient::normalize(std::string s) {
    size_t start = s.find_first_not_of(" \t\n\r\f\v!@#$%^&*()-_=+[]{}|;:',.<>?\\/~`");
    size_t end = s.find_last_not_of(" \t\n\r\f\v!@#$%^&*()-_=+[]{}|;:',.<>?\\/~`");
    
    if(start == std::string::npos || end == std::string::npos)
        return "";
    
    s = s.substr(start, end - start + 1);
    s[0] = toupper(s[0]);
    for(int i = 1; s[i] != 0; ++i)
        s[i] = tolower(s[i]);
    return s;
}

Ingredient::Ingredient(const std::string& s, const Unit& u) {
    this->name_ = this->normalize(s);
    this->unit_ = u;
}

const std::string& Ingredient::name() const {
    return this->name_;
}
Unit Ingredient::unit() const {
    return this->unit_;
}

std::ostream& operator<<(std::ostream& os, const Ingredient& ingredient){
    os << "Ingredient " << ingredient.name() << " is measured in unit " << toString(ingredient.unit()) << "\n";
    return os;
}
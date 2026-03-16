#include "../../include/core/RecipeLine.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

RecipeLine::RecipeLine() = default;

RecipeLine::RecipeLine(const Ingredient& ingredient, double amount, bool optional){
    if(amount <= 0){
        throw std::runtime_error("Ingredient amount must be positive");
    }
    this->ingredient_ = ingredient;
    this->amount_ = amount;
    this->optional_ = optional;
}

const Ingredient& RecipeLine::ingredient() const{
    return this->ingredient_;
}

double RecipeLine::amount() const{
    return this->amount_;
}

bool RecipeLine::optional() const{
    return this->optional_;
}

std::ostream& operator<<(std::ostream& os, const RecipeLine& recipeLine){
    auto formatAmount = [](double amount) -> std::string {
        if(amount == std::floor(amount)){
            return std::to_string(static_cast<int>(amount));
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << amount;
        return ss.str();
    };
    
    if(recipeLine.optional()){
        os << "This recipe suggests using " << formatAmount(recipeLine.amount()) << " " << toString(recipeLine.ingredient().unit()) << " of " << recipeLine.ingredient().name() << "\n";
    }
    else{
        os << "This recipe requires " << formatAmount(recipeLine.amount()) << " " << toString(recipeLine.ingredient().unit()) << " of " << recipeLine.ingredient().name() << "\n";
    }
    return os;
}
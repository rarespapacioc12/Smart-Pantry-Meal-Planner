#include "../../include/core/RecipeLine.hpp"
#include <stdexcept>

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

const double RecipeLine::amount() const{
    return this->amount_;
}

const bool RecipeLine::optional() const{
    return this->optional_;
}

std::ostream& operator<<(std::ostream& os, const RecipeLine& recipeLine){
    if(recipeLine.optional()){
        os << "This recipe suggests using " + std::to_string(recipeLine.amount()) + " of " + recipeLine.ingredient().name() + "\n";
    }
    else{
        os << "This recipe requires " + std::to_string(recipeLine.amount()) + " of " + recipeLine.ingredient().name() + "\n";
    }
    return os;
}
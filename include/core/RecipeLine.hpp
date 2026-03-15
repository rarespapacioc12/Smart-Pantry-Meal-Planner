#pragma once
#include "./Ingredient.hpp"
#include <iostream>

class RecipeLine{
private:
    Ingredient ingredient_;
    double amount_;
    bool optional_;

public:
    RecipeLine();
    RecipeLine(const Ingredient&, double, bool);
    const Ingredient& ingredient() const;
    const double amount() const;
    const bool optional() const;
    friend std::ostream& operator<<(std::ostream&, const RecipeLine&);
};
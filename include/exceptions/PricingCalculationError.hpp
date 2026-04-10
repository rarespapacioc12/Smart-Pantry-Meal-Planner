#pragma once
#include "./AppError.hpp"

class PricingCalculationError : public AppError{
private:
    std::string _category;
    double _attempted_price;
public:
    PricingCalculationError(const std::string&, double);
    const std::string& category() const;
    double attempted_price() const;
};

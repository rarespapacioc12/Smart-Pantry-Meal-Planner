#include "../../include/exceptions/PricingCalculationError.hpp"

PricingCalculationError::PricingCalculationError(const std::string& category, double attempted_price)
: AppError("Pricing error in " + category + " with price " + std::to_string(attempted_price) + "\n", "PRICING_ERROR"),
_category(category),
_attempted_price(attempted_price) {}

const std::string& PricingCalculationError::category() const{
    return _category;
}

double PricingCalculationError::attempted_price() const{
    return _attempted_price;
}

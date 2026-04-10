#include "../../include/exceptions/InvalidQuantityError.hpp"

InvalidQuantityError::InvalidQuantityError(double provided_quantity)
: AppError("Invalid quantity: " + std::to_string(provided_quantity), "INVALID_QUANTITY"),
_provided_quantity(provided_quantity) {}

double InvalidQuantityError::provided_quantity() const{
    return _provided_quantity;
}

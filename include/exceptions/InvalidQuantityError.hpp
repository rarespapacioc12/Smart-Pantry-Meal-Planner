#pragma once
#include "./AppError.hpp"

class InvalidQuantityError : public AppError{
private:
    double _provided_quantity;
public:
    InvalidQuantityError(double);
    double provided_quantity() const;
};

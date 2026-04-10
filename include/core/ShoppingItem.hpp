#pragma once
#include <string>
#include <iostream>
#include "./Types.hpp"
#include "../exceptions/AppError.hpp"
#include "../exceptions/InvalidQuantityError.hpp"
#include "../exceptions/PricingCalculationError.hpp"
#include "../exceptions/ItemNotFoundError.hpp"

class ShoppingItem{
private:
    std::string _item_name;
    double _quantity;
    Unit _unit;
    double _base_price;
    Priority _priority;
public:
    ShoppingItem(const std::string&, double, Unit, double, Priority);
    virtual ~ShoppingItem() = default;
    virtual double calculateTotalCost() const = 0;
    virtual std::string category() const = 0;
    virtual ShoppingItem* clone() const = 0;
    virtual std::ostream& print(std::ostream&) const;
    virtual bool canDeliver() const;
    const std::string& item_name() const;
    double quantity() const;
    Unit unit() const;
    double base_price() const;
    Priority priority() const;
    void adjustQuantity(double);
    friend std::ostream& operator<<(std::ostream&, const ShoppingItem&);
};

#pragma once
#include "./ShoppingItem.hpp"
#include "../exceptions/InvalidQuantityError.hpp"

class OrganicShoppingItem : public ShoppingItem{
private:
    Certification _certification;
    double _organic_markup;
public:
    OrganicShoppingItem(const std::string&, double, Unit,
    double, Priority, Certification, double);

    double calculateTotalCost() const override;
    std::string category() const override;
    OrganicShoppingItem* clone() const override;
    std::ostream& print(std::ostream& os) const override;

    Certification certification() const;
    double organic_markup() const;
};
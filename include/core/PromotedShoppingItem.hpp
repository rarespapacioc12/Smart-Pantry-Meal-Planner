#pragma once
#include "./ShoppingItem.hpp"
#include "../utils/Date.hpp"
#include "../exceptions/InvalidQuantityError.hpp"
#include "../exceptions/ItemNotFoundError.hpp"

class PromotedShoppingItem : public ShoppingItem {
private:
    double _discount_percentage;
    Date _promo_end;

public:
    PromotedShoppingItem(const std::string&, double, Unit,
    double, Priority, double, const Date&);

    double calculateTotalCost() const override;
    std::string category() const override;
    PromotedShoppingItem* clone() const override;
    std::ostream& print(std::ostream&) const override;

    bool canDeliver() const override;
    double discount_percentage() const;
    bool isValidPromo(const Date&) const;
};
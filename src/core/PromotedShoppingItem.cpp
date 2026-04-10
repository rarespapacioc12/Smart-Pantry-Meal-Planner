#include "../../include/core/PromotedShoppingItem.hpp"
#include "../../include/exceptions/InvalidQuantityError.hpp"
#include "../../include/exceptions/ItemNotFoundError.hpp"

PromotedShoppingItem::PromotedShoppingItem(const std::string& item_name, double quantity, Unit unit,
double base_price, Priority priority, double discount_percentage, const Date& promo_end)
: ShoppingItem(item_name, quantity, unit, base_price, priority),
_discount_percentage(discount_percentage),
_promo_end(promo_end)
{
    if(discount_percentage < 0 || discount_percentage > 100){
        throw InvalidQuantityError(discount_percentage);
    }
    if(promo_end < Date::today()){
        throw ItemNotFoundError("Promotion expired");
    }
}

double PromotedShoppingItem::calculateTotalCost() const{
    return quantity() * base_price() * (1 - _discount_percentage / 100.0);
}

std::string PromotedShoppingItem::category() const{
    return "Promoted";
}

PromotedShoppingItem* PromotedShoppingItem::clone() const{
    return new PromotedShoppingItem(*this);
}

std::ostream& PromotedShoppingItem::print(std::ostream& os) const{
    ShoppingItem::print(os);
    os << " | Discount: " << discount_percentage() << "%";
    return os;
}

bool PromotedShoppingItem::canDeliver() const{
    return Date::today() <= _promo_end;
}

double PromotedShoppingItem::discount_percentage() const{
    return _discount_percentage;
}

bool PromotedShoppingItem::isValidPromo(const Date& current_date) const{
    return current_date <= _promo_end;
}
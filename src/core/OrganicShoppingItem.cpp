#include "../../include/core/OrganicShoppingItem.hpp"

OrganicShoppingItem::OrganicShoppingItem(const std::string& item_name, double quantity, 
Unit unit, double base_price, Priority priority, Certification certification, double organic_markup)
: ShoppingItem(item_name, quantity, unit, base_price, priority),
_certification(certification),
_organic_markup(organic_markup)
{
    if(organic_markup < 0 || organic_markup > 100){
        throw InvalidQuantityError(organic_markup);
    }
}

double OrganicShoppingItem::calculateTotalCost() const{
    return quantity() * base_price() * (1 + _organic_markup / 100.0);
}

std::string OrganicShoppingItem::category() const{
    return "Organic";
}

OrganicShoppingItem* OrganicShoppingItem::clone() const{
    return new OrganicShoppingItem(*this);
}

std::ostream& OrganicShoppingItem::print(std::ostream& os) const{
    ShoppingItem::print(os);
    os << " | Organic (+" << organic_markup() << "%) | Cert: " << toString(certification());
    return os;
}

Certification OrganicShoppingItem::certification() const{
    return _certification;
}

double OrganicShoppingItem::organic_markup() const{
    return _organic_markup;
}
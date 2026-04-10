#include "../../include/core/ShoppingItem.hpp"

ShoppingItem::ShoppingItem(const std::string& item_name, double quantity, Unit unit, double base_price, Priority priority)
: _item_name(item_name), _quantity(quantity), _unit(unit), _base_price(base_price), _priority(priority){
    if(quantity <= 0){
        throw InvalidQuantityError(quantity);
    }
    if(base_price < 0){
        throw PricingCalculationError(item_name, base_price);
    }
}

std::ostream& ShoppingItem::print(std::ostream& os) const{
    os << _item_name << " | " << _quantity << " " << toString(_unit) << " | " << _base_price << " lei | Priority " << toString(_priority);
    return os;
}

const std::string& ShoppingItem::item_name() const{
    return _item_name;
}

double ShoppingItem::quantity() const{
    return _quantity;
}

Unit ShoppingItem::unit() const{
    return _unit;
}

double ShoppingItem::base_price() const{
    return _base_price;
}

Priority ShoppingItem::priority() const{
    return _priority;
}

void ShoppingItem::adjustQuantity(double new_quantity){
    if(new_quantity < 0){
        throw InvalidQuantityError(new_quantity);
    }
    _quantity = new_quantity;
}

bool ShoppingItem::canDeliver() const{
    return true;
}

std::ostream& operator<<(std::ostream& os, const ShoppingItem& shoppingItem){
    shoppingItem.print(os);
    return os;
}

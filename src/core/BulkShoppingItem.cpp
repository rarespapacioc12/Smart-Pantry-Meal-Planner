#include "../../include/core/BulkShoppingItem.hpp"

BulkShoppingItem::BulkShoppingItem(const std::string& item_name, double quantity, Unit unit,
double base_price, Priority priority, double min_quantity, double bulk_discount)
: ShoppingItem(item_name, quantity, unit, base_price, priority),
_min_quantity(min_quantity),
_bulk_discount(bulk_discount)
{
    if(min_quantity <= 0){
        throw InvalidQuantityError(min_quantity);
    }
    if(bulk_discount < 0 || bulk_discount > 100){
        throw InvalidQuantityError(bulk_discount);
    }
}
    
double BulkShoppingItem::calculateTotalCost() const{
    if(meetsMinimum()){
        return quantity() * base_price() * (1 - _bulk_discount / 100.0);
    }
    return quantity() * base_price() * 1.15;
}

std::string BulkShoppingItem::category() const{
    return "Bulk";
}

BulkShoppingItem* BulkShoppingItem::clone() const{
    return new BulkShoppingItem(*this);
}

std::ostream& BulkShoppingItem::print(std::ostream& os) const{
    ShoppingItem::print(os);
    os << " | Min Qty: " << min_quantity() << " | Bulk Discount: " << _bulk_discount << "%";
    return os;
}

bool BulkShoppingItem::canDeliver() const{
    return meetsMinimum();    
}
double BulkShoppingItem::min_quantity() const{
    return _min_quantity;
}
bool BulkShoppingItem::meetsMinimum() const{
    return quantity() >= _min_quantity;
}
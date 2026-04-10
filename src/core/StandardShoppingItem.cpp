#include "../../include/core/StandardShoppingItem.hpp"
#include "../../include/exceptions/AppError.hpp"

StandardShoppingItem::StandardShoppingItem(const std::string& item_name, double quantity, Unit unit, double base_price, Priority priority, const std::string& supplier)
: ShoppingItem(item_name, quantity, unit, base_price, priority), _supplier(supplier){
    if(supplier.empty()){
        throw AppError("Supplier may not be empty!", "EMPTY_SUPPLIER");
    }
}

double StandardShoppingItem::calculateTotalCost() const{
    return quantity() * base_price();
}

std::string StandardShoppingItem::category() const{
    return "Standard";
}

StandardShoppingItem* StandardShoppingItem::clone() const{
    return new StandardShoppingItem(*this);
}

std::ostream& StandardShoppingItem::print(std::ostream& os) const{
    ShoppingItem::print(os);
    os << " | Supplier: " << supplier();
    return os;
}

const std::string& StandardShoppingItem::supplier() const{
    return _supplier;
}

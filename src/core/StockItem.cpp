#include "../../include/core/StockItem.hpp"
#include <stdexcept>

StockItem::StockItem(const Ingredient& ingredient, double quantity, const Date& date, const Location& location){
    if(quantity <= 0){
        throw std::runtime_error("Quantity must be positive");
    }
    this->ingredient_ = ingredient;
    this->quantity_ = quantity;
    this->expiry_ = date;
    this->location_ = location;
}

const Ingredient& StockItem::ingredient() const{
    return this->ingredient_;
}

double StockItem::quantity() const{
    return this->quantity_;
}

const Date& StockItem::expiry() const{
    return this->expiry_;
}

const Location& StockItem::location() const{
    return this->location_;
}

void StockItem::consume(double amount){
    if(amount <= 0){
        throw std::runtime_error("Amount consumed may not be negative");
    }
    if(this->quantity_ < amount){
        throw std::runtime_error("Amount bigger than available supply");
    }
    this->quantity_ -= amount;
}

bool StockItem::isExpired(const Date& today) const{
    return !(today < this->expiry_);
}

int StockItem::daysToExpire(const Date& today) const{
    return today.daysUntil(this->expiry_);
}

std::ostream& operator<<(std::ostream& os, const StockItem& stockItem){
    os << stockItem.ingredient();
    os << "It expires on " + stockItem.expiry().toISO() + ", has an available quantity of " + std::to_string(stockItem.quantity()) + toString(stockItem.ingredient().unit()) + " and it is located in the " + toString(stockItem.location()) + ".\n";
    return os;
}
#include "../../include/core/Inventory.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>

Inventory::Inventory() = default;

Inventory::Inventory(const Inventory& other){
    this->items_ = other.items_;
}

Inventory& Inventory::operator=(const Inventory& other){
    this->items_ = other.items_;
    return *this;
}

void Inventory::add(const StockItem& stockItem){
    this->items_.push_back(stockItem);
}

bool Inventory::removeFirstByName(const std::string& name){
    std::vector<StockItem>::iterator it = this->items_.begin();
    while(it != this->items_.end() && it->ingredient().name() != name){
        ++it;
    }

    if(it == this->items_.end()){
        return false;
    }

    this->items_.erase(it);
    return true;
}

double Inventory::available(const std::string& name) const {
    double quant = 0;
    
    for(const StockItem& stockItem : this->items_){
        if(stockItem.ingredient().name() == name){
            quant += stockItem.quantity();
        }
    }

    return quant;
}

std::vector<StockItem> Inventory::expiringSoon(int days, const Date& today){
    std::vector<StockItem> expiring_soon;

    for(const StockItem& stockItem : this->items_){
        if(stockItem.daysToExpire(today) <= days){
            expiring_soon.push_back(stockItem);
        }
    }

    return expiring_soon;
}

void Inventory::consumeByExpiry(const std::string& ingredientName, double amount){
    std::vector<std::vector<StockItem>::iterator> consumeList;

    for(std::vector<StockItem>::iterator I = this->items_.begin(); I != this->items_.end(); ++I){
        if(I->ingredient().name() == ingredientName){
            consumeList.push_back(I);
        }
    }

    std::sort(consumeList.begin(), consumeList.end(), [](const StockItem& a, const StockItem& b){
        return a.expiry() < b.expiry();
    });

    for(auto it = consumeList.begin(); it != consumeList.end() && amount > 0; ++it){
        if((*it)->quantity() >= amount){
            (*it)->consume(amount);
            amount = 0;
        }
        else{
            amount -= (*it)->quantity();
            (*it)->consume((*it)->quantity());
        }
    }

    if(amount > 0){
        throw std::runtime_error("Not enough quantity of " + ingredientName);
    }

    this->items_.erase(
        std::remove_if(this->items_.begin(), this->items_.end(),
            [](const StockItem& item) { return item.quantity() == 0; }),
        this->items_.end()
    );
}

const std::vector<StockItem>& Inventory::items() const{
    return this->items_;
}

std::ostream& operator<<(std::ostream& os, const Inventory& inventory){
    for(const StockItem& stockItem : inventory.items()){
        os << stockItem;
    }
    return os;
}

Inventory::~Inventory() = default;
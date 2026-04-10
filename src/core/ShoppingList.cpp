#include "../../include/core/ShoppingList.hpp"
#include <iostream>

ShoppingList::ShoppingList(){
    _items = {};
    _create_date = Date::today();
    _list_count++;
}

ShoppingList::ShoppingList(const ShoppingList& other){
    _items = other._items;
    _create_date = other._create_date;
    _list_count++;
}

ShoppingList& ShoppingList::operator=(ShoppingList& other){
    swap(*this, other);
    return *this;
}

ShoppingList::~ShoppingList(){
    --_list_count;
}

void ShoppingList::addItem(std::shared_ptr<ShoppingItem> item){
    if(item == nullptr){
        throw ItemNotFoundError(item->item_name());
    }
    if(item->quantity() <= 0){
        throw InvalidQuantityError(item->quantity());
    }
    _items.push_back(item);
}

bool ShoppingList::removeItem(const std::string& name){
    std::vector<std::shared_ptr<ShoppingItem> >::iterator it = _items.begin();
    while(it != _items.end() && (*it)->item_name() != name){
        ++it;
    }
    if(it == _items.end()){
        return false;
    }
    _items.erase(it);
    return true;
}

double ShoppingList::calculateTotalCost() const{
    double total_cost = 0;
    for(const auto& item : _items){
        total_cost += item->calculateTotalCost();
    }
    return total_cost;
}

double ShoppingList::calculateByCategory(const std::string& category) const{
    double total_cost = 0;
    for(const auto& item : _items){
        if(item->category() == category){
            total_cost += item->calculateTotalCost();
        }
    }
    return total_cost;
}

std::vector<std::shared_ptr<ShoppingItem> > ShoppingList::getPriorityItems(Priority min_priority) const{
    std::vector<std::shared_ptr<ShoppingItem> > sol;
    for(const auto& item : _items){
        if(item->priority() >= min_priority){
            sol.push_back(item);
        }
    }
    return sol;
}

std::vector<std::shared_ptr<ShoppingItem> > ShoppingList::getUndiscountedItems() const{
    std::vector<std::shared_ptr<ShoppingItem> > sol;
    for(const auto& item : _items){
        if(dynamic_cast<PromotedShoppingItem*>(item.get()) == nullptr){
            sol.push_back(item);
        }
    }
    return sol;
}

void ShoppingList::applySpecialOffer(double extra_discount) const{
    for(const auto& item : _items){
        PromotedShoppingItem* promo = dynamic_cast<PromotedShoppingItem*>(item.get());
        if(promo != nullptr){
            std::cout << "Promo on " << promo->item_name() << ": " 
                      << promo->discount_percentage() << "% + " << extra_discount 
                      << "% extra = " << (promo->discount_percentage() + extra_discount) << "%\n";
        }
        
        BulkShoppingItem* bulk = dynamic_cast<BulkShoppingItem*>(item.get());
        if(bulk != nullptr){
            if(!bulk->meetsMinimum()){
                std::cout << "Warning: " << bulk->item_name() << " (" << bulk->quantity() 
                          << ") does not meet minimum (" << bulk->min_quantity() << ")\n";
            }
        }
    }
}

void ShoppingList::displayAll() const{
    for(const auto& item : _items){
        if(dynamic_cast<StandardShoppingItem*>(item.get()) != nullptr){
            std::cout << "[Standard] " << item;   
        }
        else if(dynamic_cast<PromotedShoppingItem*>(item.get()) != nullptr){
            std::cout << "[Promoted] " << item;
        }
        else if(dynamic_cast<BulkShoppingItem*>(item.get()) != nullptr){
            std::cout << "[Bulk] " << item;
        }
        else if(dynamic_cast<OrganicShoppingItem*>(item.get()) != nullptr){
            std::cout << "[Organic] " << item;
        }
    }
}

void ShoppingList::displayByCategory() const{
    for(const auto& item : _items){
        if(dynamic_cast<StandardShoppingItem*>(item.get()) != nullptr){
            std::cout << "[Standard] " << item;   
        }
    }
    for(const auto& item : _items){
        if(dynamic_cast<PromotedShoppingItem*>(item.get()) != nullptr){
            std::cout << "[Promoted] " << item;
        }
    }
    for(const auto& item : _items){
        if(dynamic_cast<BulkShoppingItem*>(item.get()) != nullptr){
            std::cout << "[Bulk] " << item;
        }
    }
    for(const auto& item : _items){
        if(dynamic_cast<OrganicShoppingItem*>(item.get()) != nullptr){
            std::cout << "[Organic] " << item;
        }
    }
}

int ShoppingList::getTotalListsCreated(){
    return _list_count;
}

void ShoppingList::resetListCount(){
    _list_count = 0;
}

std::shared_ptr<ShoppingItem> ShoppingList::createItem(const std::string& type, const std::string& name, double qty, Unit unit, double price, Priority priority, double param1, const Date& param2, double param3, Certification cert, const std::string& supplier){
    if(type == "Standard"){
        return std::make_shared<StandardShoppingItem>(name, qty, unit, price, priority, supplier);
    }
    else if(type == "Promoted"){
        return std::make_shared<PromotedShoppingItem>(name, qty, unit, price, priority, param1, param2);
    }
    else if(type == "Bulk"){
        return std::make_shared<BulkShoppingItem>(name, qty, unit, price, priority, param1, param3);
    }
    else if(type == "Organic"){
        return std::make_shared<OrganicShoppingItem>(name, qty, unit, price, priority, cert, param1);
    }
    throw ItemNotFoundError(type);
}

void swap(ShoppingList& a, ShoppingList& b){
    std::swap(a._items, b._items);
    std::swap(a._create_date, b._create_date);
}

int ShoppingList::_list_count = 0;

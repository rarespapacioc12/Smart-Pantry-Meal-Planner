#include "../../include/core/ShoppingList.hpp"
#include <iostream>

ShoppingList::ShoppingList() : _items(), _create_date(Date::today()){
    ++_list_count;
}

ShoppingList::ShoppingList(const ShoppingList& other) : _items(), _create_date(other._create_date){
    _items.reserve(other._items.size());
    for(const auto& item : other._items){
        _items.emplace_back(item->clone());
    }
    ++_list_count;
}

ShoppingList& ShoppingList::operator=(ShoppingList other){
    swap(*this, other);
    return *this;
}

ShoppingList::~ShoppingList(){
    --_list_count;
}

void ShoppingList::addItem(const std::shared_ptr<ShoppingItem>& item){
    if(item == nullptr){
        throw ItemNotFoundError("<null item>");
    }
    if(item->quantity() <= 0){
        throw InvalidQuantityError(item->quantity());
    }
    _items.push_back(item);
}

bool ShoppingList::removeItem(const std::string& name){
    auto it = _items.begin();
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
        if(std::dynamic_pointer_cast<PromotedShoppingItem>(item) == nullptr){
            sol.push_back(item);
        }
    }
    return sol;
}

void ShoppingList::applySpecialOffer(double extra_discount) const{
    for(const auto& item : _items){
        if(auto promo = std::dynamic_pointer_cast<PromotedShoppingItem>(item)){
            std::cout << "Promo on " << promo->item_name() << ": "
                      << promo->discount_percentage() << "% + " << extra_discount
                      << "% extra = " << (promo->discount_percentage() + extra_discount) << "%\n";
        }
        if(auto bulk = std::dynamic_pointer_cast<BulkShoppingItem>(item)){
            if(!bulk->meetsMinimum()){
                std::cout << "Warning: " << bulk->item_name() << " (" << bulk->quantity()
                          << ") does not meet minimum (" << bulk->min_quantity() << ")\n";
            }
        }
    }
}

void ShoppingList::displayAll() const{
    for(const auto& item : _items){
        std::cout << "[" << item->category() << "] " << *item << '\n';
    }
}

void ShoppingList::displayByCategory() const{
    const std::string categories[] = {"Standard", "Promoted", "Bulk", "Organic"};
    for(const auto& cat : categories){
        for(const auto& item : _items){
            if(item->category() == cat){
                std::cout << "[" << cat << "] " << *item << '\n';
            }
        }
    }
    for(const auto& item : _items){
        const std::string& cat = item->category();
        if(cat != "Standard" && cat != "Promoted" && cat != "Bulk" && cat != "Organic"){
            std::cout << "[" << cat << "] " << *item << '\n';
        }
    }
}

std::size_t ShoppingList::size() const{
    return _items.size();
}

const Date& ShoppingList::createDate() const{
    return _create_date;
}

int ShoppingList::getTotalListsCreated(){
    return _list_count;
}

void ShoppingList::resetListCount(){
    _list_count = 0;
}

std::shared_ptr<ShoppingItem> ShoppingList::createItem(const std::string& type, const std::string& name, double qty, Unit unit, double price, Priority priority, double param1, const Date& param2, double param3, Certification cert, const std::string& supplier, Season season){
    if(type == "Standard"){
        return std::make_shared<StandardShoppingItem>(name, qty, unit, price, priority, supplier);
    }
    if(type == "Promoted"){
        return std::make_shared<PromotedShoppingItem>(name, qty, unit, price, priority, param1, param2);
    }
    if(type == "Bulk"){
        return std::make_shared<BulkShoppingItem>(name, qty, unit, price, priority, param1, param3);
    }
    if(type == "Organic"){
        return std::make_shared<OrganicShoppingItem>(name, qty, unit, price, priority, cert, param1);
    }
    if(type == "Seasonal"){
        return std::make_shared<SeasonalShoppingItem>(name, qty, unit, price, priority, season, param1, param3, param2);
    }
    throw ItemNotFoundError(type);
}

void swap(ShoppingList& a, ShoppingList& b){
    using std::swap;
    swap(a._items, b._items);
    swap(a._create_date, b._create_date);
}

std::ostream& operator<<(std::ostream& os, const ShoppingList& list){
    os << "Shopping list (" << list._items.size() << " items, created " << list._create_date.toISO() << "):\n";
    for(const auto& item : list._items){
        os << "  [" << item->category() << "] " << *item << '\n';
    }
    return os;
}

int ShoppingList::_list_count = 0;

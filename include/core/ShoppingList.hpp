#pragma once
#include <vector>
#include <memory>
#include "./ShoppingItem.hpp"
#include "./StandardShoppingItem.hpp"
#include "./PromotedShoppingItem.hpp"
#include "./OrganicShoppingItem.hpp"
#include "./BulkShoppingItem.hpp"
#include "../utils/Date.hpp"
#include "../exceptions/ItemNotFoundError.hpp"
#include "../exceptions/InvalidQuantityError.hpp"

class ShoppingList{
private:
    std::vector<std::shared_ptr<ShoppingItem> > _items;
    Date _create_date;
    static int _list_count;
public:
    ShoppingList();
    ShoppingList(const ShoppingList&);
    ShoppingList& operator=(ShoppingList&);
    ~ShoppingList();
    void addItem(std::shared_ptr<ShoppingItem>);
    bool removeItem(const std::string&);
    double calculateTotalCost() const;
    double calculateByCategory(const std::string&) const;
    std::vector<std::shared_ptr<ShoppingItem> > getPriorityItems(Priority) const;
    std::vector<std::shared_ptr<ShoppingItem> > getUndiscountedItems() const;
    void applySpecialOffer(double) const;
    void displayAll() const;
    void displayByCategory() const;
    static int getTotalListsCreated();
    static void resetListCount();
    static std::shared_ptr<ShoppingItem> createItem(const std::string&, const std::string&, double, Unit, double, Priority, double, const Date& param2 = Date::today(), double param3 = 0, Certification cert = Certification::EUOrganic, const std::string& supplier = "Generic");
    friend void swap(ShoppingList& a, ShoppingList& b);
};

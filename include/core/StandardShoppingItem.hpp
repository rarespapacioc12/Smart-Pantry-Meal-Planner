#pragma once
#include "./ShoppingItem.hpp"

class StandardShoppingItem : public ShoppingItem{
private:
    std::string _supplier;
public:
    StandardShoppingItem(const std::string&, double, Unit, double, Priority, const std::string&);
    double calculateTotalCost() const override;
    std::string category() const override;
    StandardShoppingItem* clone() const override;
    std::ostream& print(std::ostream&) const override;
    const std::string& supplier() const;
};
#pragma once
#include "./ShoppingItem.hpp"

class StandardShoppingItem : public ShoppingItem {
private:
    std::string _supplier;
public:
    StandardShoppingItem(const std::string&, double, Unit, double, Priority, const std::string&);

    double calculateTotalCost() const override;
    std::string category() const override;
    StandardShoppingItem* clone() const override;
    std::ostream& print(std::ostream&) const override;
    const std::string& supplier() const;
};
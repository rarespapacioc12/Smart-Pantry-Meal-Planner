#pragma once
#include "./ShoppingItem.hpp"

class BulkShoppingItem : public ShoppingItem{
private:
    double _min_quantity;
    double _bulk_discount;
public:
    BulkShoppingItem(const std::string&, double, Unit, double, Priority, double, double);
    double calculateTotalCost() const override;
    std::string category() const override;
    BulkShoppingItem* clone() const override;
    std::ostream& print(std::ostream&) const override;
    bool canDeliver() const override;
    double min_quantity() const;
    double bulk_discount() const;
    bool meetsMinimum() const;
};
#pragma once
#include "./ShoppingItem.hpp"

class BulkShoppingItem : public ShoppingItem {
private:
    double _min_quantity;
    double _bulk_discount;

public:
    BulkShoppingItem(const std::string&, double, Unit,
    double, Priority, double, double);
    
    double calculateTotalCost() const override;
    std::string category() const override;
    BulkShoppingItem* clone() const override;
    std::ostream& print(std::ostream&) const override;

    bool canDeliver() const override;
    double min_quantity() const;
    bool meetsMinimum() const;
};
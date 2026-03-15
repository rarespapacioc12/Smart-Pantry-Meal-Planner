#pragma once
#include "./Ingredient.hpp"
#include "../utils/Date.hpp"
#include "./Types.hpp"

class StockItem{
private:
    Ingredient ingredient_;
    double quantity_;
    Date expiry_;
    Location location_;

public:
    StockItem(const Ingredient&, double, const Date&, const Location&);
    const Ingredient& ingredient() const;
    double quantity() const;
    const Date& expiry() const;
    const Location& location() const;
    void consume(double);
    bool isExpired(const Date&) const;
    int daysToExpire(const Date&) const;
    friend std::ostream& operator<<(std::ostream&, const StockItem&);
};
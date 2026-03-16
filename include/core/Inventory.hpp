#pragma once
#include "./StockItem.hpp"
#include <vector>

class Inventory{
private:
    std::vector<StockItem> items_;

public:
    Inventory();
    Inventory(const Inventory&);
    Inventory& operator=(const Inventory&);

    void add(const StockItem&);
    bool removeFirstByName(const std::string&);
    double available(const std::string&) const;
    std::vector<StockItem> expiringSoon(int, const Date&);
    void consumeByExpiry(const std::string&, double);
    const std::vector<StockItem>& items() const;
    friend std::ostream& operator<<(std::ostream&, const Inventory&);

    ~Inventory();
};
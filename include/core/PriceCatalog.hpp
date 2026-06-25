#pragma once
#include <map>
#include <string>

class PriceCatalog {
private:
    std::map<std::string, double> _prices;
    std::string _currency;

public:
    PriceCatalog() = default;

    void load(const std::string& path);

    double priceOf(const std::string& ingredient) const;
    bool has(const std::string& ingredient) const;
    const std::string& currency() const;
};

#pragma once
#include <map>
#include <string>

// ============================================================================
// Tema 3 - Catalog de preturi citit din data/prices.json (nlohmann/json).
// ----------------------------------------------------------------------------
// Sursa unica de preturi pentru ingredientele cumparate automat de MealPlanner,
// astfel incat niciun pret sa nu fie hardcodat in cod.
// ============================================================================
class PriceCatalog {
private:
    std::map<std::string, double> _prices;
    std::string _currency;

public:
    PriceCatalog() = default;

    void load(const std::string& path);

    // Pretul pe unitate al unui ingredient; arunca ItemNotFoundError daca lipseste.
    double priceOf(const std::string& ingredient) const;
    bool has(const std::string& ingredient) const;
    const std::string& currency() const;
};

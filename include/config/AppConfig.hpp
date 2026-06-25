#pragma once
#include <string>
#include "../utils/Date.hpp"

// ============================================================================
// Tema 3 - DESIGN PATTERN: Singleton (+ biblioteca externa nlohmann/json)
// ----------------------------------------------------------------------------
// Stare globala de configurare a aplicatiei, citita EXCLUSIV din data/config.json.
// Inlocuieste constantele hardcodate (cai de fisiere, data curenta, parametri de
// planificare) care existau in `namespace { ... }` din main.cpp.
// Nicio valoare de business nu mai apare in cod.
// ============================================================================
class AppConfig {
public:
    static AppConfig& instance();

    // Incarca si valideaza configuratia dintr-un fisier JSON (nlohmann).
    void load(const std::string& path);

    // --- Fisiere de intrare ---
    const std::string& inventoryIn() const;
    const std::string& recipesIn() const;
    const std::string& shoppingIn() const;
    const std::string& pricesIn() const;

    // --- Fisiere de iesire ---
    const std::string& inventoryOut() const;
    const std::string& recipesOut() const;
    const std::string& shoppingOut() const;
    const std::string& mealPlanOut() const;

    // --- Parametri de planificare ---
    const Date& today() const;
    int topRecipes() const;
    int expiryWindowDays() const;
    int portions() const;
    const std::string& defaultSupplier() const;

    // --- Logging ---
    const std::string& logLevel() const;
    const std::string& logFile() const;

    // Permit suprascrierea catorva valori din argumentele liniei de comanda (cxxopts).
    void overridePortions(int portions);

    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

private:
    AppConfig() = default;
    ~AppConfig() = default;

    std::string _inventoryIn;
    std::string _recipesIn;
    std::string _shoppingIn;
    std::string _pricesIn;

    std::string _inventoryOut;
    std::string _recipesOut;
    std::string _shoppingOut;
    std::string _mealPlanOut;

    Date _today;
    int _topRecipes = 3;
    int _expiryWindowDays = 20;
    int _portions = 1;
    std::string _defaultSupplier;

    std::string _logLevel = "info";
    std::string _logFile;
};

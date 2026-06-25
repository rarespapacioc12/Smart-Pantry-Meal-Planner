#pragma once
#include <string>
#include "../utils/Date.hpp"

class AppConfig {
public:
    static AppConfig& instance();

    void load(const std::string& path);

    const std::string& inventoryIn() const;
    const std::string& recipesIn() const;
    const std::string& shoppingIn() const;
    const std::string& pricesIn() const;

    const std::string& inventoryOut() const;
    const std::string& recipesOut() const;
    const std::string& shoppingOut() const;
    const std::string& mealPlanOut() const;

    const Date& today() const;
    int topRecipes() const;
    int expiryWindowDays() const;
    int portions() const;
    const std::string& defaultSupplier() const;

    const std::string& logLevel() const;
    const std::string& logFile() const;

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

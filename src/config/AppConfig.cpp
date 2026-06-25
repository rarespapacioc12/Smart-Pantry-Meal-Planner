#include "../../include/config/AppConfig.hpp"
#include "../../include/exceptions/AppError.hpp"

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

AppConfig& AppConfig::instance() {
    static AppConfig s_instance;
    return s_instance;
}

void AppConfig::load(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw AppError("Cannot open config file: " + path, "CONFIG_IO");
    }

    nlohmann::json j;
    try {
        in >> j;
    } catch (const nlohmann::json::parse_error& e) {
        throw AppError(std::string("Config parse error: ") + e.what(), "CONFIG_PARSE");
    }

    const auto& dataFiles = j.at("dataFiles");
    _inventoryIn = dataFiles.at("inventory").get<std::string>();
    _recipesIn   = dataFiles.at("recipes").get<std::string>();
    _shoppingIn  = dataFiles.at("shoppingList").get<std::string>();
    _pricesIn    = dataFiles.at("prices").get<std::string>();

    const auto& out = j.at("output");
    _inventoryOut = out.at("inventory").get<std::string>();
    _recipesOut   = out.at("recipes").get<std::string>();
    _shoppingOut  = out.at("shoppingList").get<std::string>();
    _mealPlanOut  = out.at("mealPlan").get<std::string>();

    const auto& planner = j.at("planner");
    _today            = Date::fromISO(planner.at("today").get<std::string>());
    _topRecipes       = planner.at("topRecipes").get<int>();
    _expiryWindowDays = planner.at("expiryWindowDays").get<int>();
    _portions         = planner.at("portions").get<int>();
    _defaultSupplier  = planner.at("defaultSupplier").get<std::string>();

    const auto& logging = j.at("logging");
    _logLevel = logging.at("level").get<std::string>();
    _logFile  = logging.at("file").get<std::string>();
}

const std::string& AppConfig::inventoryIn() const { return _inventoryIn; }
const std::string& AppConfig::recipesIn() const { return _recipesIn; }
const std::string& AppConfig::shoppingIn() const { return _shoppingIn; }
const std::string& AppConfig::pricesIn() const { return _pricesIn; }

const std::string& AppConfig::inventoryOut() const { return _inventoryOut; }
const std::string& AppConfig::recipesOut() const { return _recipesOut; }
const std::string& AppConfig::shoppingOut() const { return _shoppingOut; }
const std::string& AppConfig::mealPlanOut() const { return _mealPlanOut; }

const Date& AppConfig::today() const { return _today; }
int AppConfig::topRecipes() const { return _topRecipes; }
int AppConfig::expiryWindowDays() const { return _expiryWindowDays; }
int AppConfig::portions() const { return _portions; }
const std::string& AppConfig::defaultSupplier() const { return _defaultSupplier; }

const std::string& AppConfig::logLevel() const { return _logLevel; }
const std::string& AppConfig::logFile() const { return _logFile; }

void AppConfig::overridePortions(int portions) {
    if (portions > 0) {
        _portions = portions;
    }
}

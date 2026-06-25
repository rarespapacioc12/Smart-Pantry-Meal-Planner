#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <cxxopts.hpp>

#include "include/config/AppConfig.hpp"
#include "include/utils/Logger.hpp"
#include "include/templates/Repository.hpp"
#include "include/templates/Algorithms.hpp"
#include "include/factory/ShoppingItemFactory.hpp"

#include "include/core/Inventory.hpp"
#include "include/core/StockItem.hpp"
#include "include/core/Recipe.hpp"
#include "include/core/RecipeBook.hpp"
#include "include/core/PriceCatalog.hpp"
#include "include/core/MealPlanner.hpp"
#include "include/core/ShoppingList.hpp"
#include "include/core/StandardShoppingItem.hpp"
#include "include/core/BulkShoppingItem.hpp"

#include "include/io/DomainJson.hpp"
#include "include/io/JsonReader.hpp"

#include "include/exceptions/AppError.hpp"
#include "include/exceptions/InvalidQuantityError.hpp"
#include "include/exceptions/ItemNotFoundError.hpp"
#include "include/exceptions/PricingCalculationError.hpp"
#include "include/exceptions/JsonParseError.hpp"

static void demoTemplates(const Inventory& pantry, const RecipeBook& book, const AppConfig& cfg) {
    std::cout << "===== TEMPLATES (Repository<T> + algoritmi generici) =====\n\n";

    Repository<StockItem> stockRepo(pantry.items());
    Repository<Recipe>    recipeRepo(book.all());

    Logger::instance().info("Repository<StockItem> size=" + std::to_string(stockRepo.size()) +
                            ", Repository<Recipe> size=" + std::to_string(recipeRepo.size()));

    const Date& today = cfg.today();
    const int window = cfg.expiryWindowDays();

    const auto expiryKey = [&](const StockItem& s) -> int {
        return s.isExpired(today) ? -1 : s.daysToExpire(today);
    };

    const std::vector<StockItem> expiring = filterItems(
        stockRepo.all(),
        [&](const StockItem& s) { return expiryKey(s) <= window; });
    std::cout << "Stock items expiring within " << window << " days (incl. expired): "
              << expiring.size() << '\n';

    const std::vector<Recipe> cookable = filterItems(
        recipeRepo.all(),
        [&](const Recipe& r) { return r.missing(pantry, cfg.portions()).empty(); });
    std::cout << "Recipes fully cookable now (" << cfg.portions()
              << " portions): " << cookable.size() << '\n';

    if (const StockItem* soonest = minBy(stockRepo.all(), expiryKey)) {
        std::cout << "Soonest to expire: " << soonest->ingredient().name()
                  << " (expires " << soonest->expiry().toISO() << ")\n";
    }
    if (const Recipe* quickest = minBy(recipeRepo.all(),
            [](const Recipe& r) { return r.prepMinutes(); })) {
        std::cout << "Quickest recipe: " << quickest->title()
                  << " (" << quickest->prepMinutes() << " min)\n";
    }
    std::cout << '\n';
}

static void demoMealPlan(const Inventory& pantry, const RecipeBook& book,
                         const PriceCatalog& prices, const AppConfig& cfg) {
    std::cout << "===== MEAL PLAN (Factory + auto shopping list) =====\n\n";

    const ShoppingItemFactory factory;
    const MealPlanner planner(pantry, book, prices, factory);

    const ShoppingList autoList = planner.planShoppingForTopRecipes(
        cfg.topRecipes(), cfg.portions(), cfg.defaultSupplier());

    std::cout << "Auto-generated shopping list for missing ingredients:\n";
    std::cout << autoList << '\n';
    std::cout << "Estimated cost: " << autoList.calculateTotalCost()
              << ' ' << prices.currency() << "\n\n";

    planner.exportPlan(cfg.mealPlanOut(), cfg.topRecipes(), cfg.portions(), cfg.defaultSupplier());
    std::cout << "Meal plan exported (nlohmann/json) to " << cfg.mealPlanOut() << "\n\n";
    Logger::instance().info("Meal plan exported to " + cfg.mealPlanOut());
}

static void demoShoppingListFromJson(const AppConfig& cfg) {
    std::cout << "===== SHOPPING LIST (loaded from JSON) =====\n\n";

    ShoppingList list = DomainJson::loadShoppingList(cfg.shoppingIn());
    std::cout << "Loaded " << list.size() << " items.\n";
    std::cout << "Total cost: "        << list.calculateTotalCost()            << " lei\n";
    std::cout << "Organic subtotal: "  << list.calculateByCategory("Organic")  << " lei\n";
    std::cout << "Seasonal subtotal: " << list.calculateByCategory("Seasonal") << " lei\n\n";

    DomainJson::saveShoppingList(cfg.shoppingOut(), list);
    std::cout << "Saved (round-trip) to " << cfg.shoppingOut() << "\n\n";
}

static void demoExceptions() {
    std::cout << "===== EXCEPTION HIERARCHY DEMO =====\n\n";
    try {
        StandardShoppingItem bad("Bread", -1, Unit::pcs, 2.0, Priority::Normal, "Local Bakery");
        (void)bad;
    } catch (const InvalidQuantityError& e) {
        std::cout << "Caught InvalidQuantityError: " << e.what() << " [code=" << e.code() << "]\n";
    }
    try {
        auto item = ShoppingList::createItem("UnknownType", "X", 1, Unit::g, 1, Priority::Low, 0);
        (void)item;
    } catch (const ItemNotFoundError& e) {
        std::cout << "Caught ItemNotFoundError: " << e.what() << " [code=" << e.code() << "]\n";
    }
    try {
        (void)JsonReader::parse("{ \"items\": [1, 2, , 3] }");
    } catch (const JsonParseError& e) {
        std::cout << "Caught JsonParseError: " << e.what() << " [code=" << e.code() << "]\n";
    }
    std::cout << '\n';
}

int main(int argc, char** argv) {
    try {
        cxxopts::Options options("smartpantry", "Smart Pantry Meal Planner");
        options.add_options()
            ("c,config",   "Path to config JSON",
                cxxopts::value<std::string>()->default_value("data/config.json"))
            ("p,portions", "Override number of portions to plan for",
                cxxopts::value<int>())
            ("v,verbose",  "Enable debug logging",
                cxxopts::value<bool>()->default_value("false"))
            ("h,help",     "Print usage");
        const cxxopts::ParseResult args = options.parse(argc, argv);

        if (args.count("help")) {
            std::cout << options.help() << '\n';
            return 0;
        }

        AppConfig& cfg = AppConfig::instance();
        cfg.load(args["config"].as<std::string>());
        if (args.count("portions")) {
            cfg.overridePortions(args["portions"].as<int>());
        }

        Logger::instance().init(args["verbose"].as<bool>() ? "debug" : cfg.logLevel(),
                                cfg.logFile());
        Logger::instance().info("Smart Pantry started; config loaded.");

        const Inventory   pantry = DomainJson::loadInventory(cfg.inventoryIn());
        const RecipeBook  book   = DomainJson::loadRecipeBook(cfg.recipesIn());
        PriceCatalog prices;
        prices.load(cfg.pricesIn());
        Logger::instance().info("Loaded " + std::to_string(pantry.items().size()) +
                                " stock items and " + std::to_string(book.all().size()) + " recipes.");

        demoTemplates(pantry, book, cfg);
        demoMealPlan(pantry, book, prices, cfg);
        demoShoppingListFromJson(cfg);
        demoExceptions();

        std::cout << "===== DEMO COMPLETED SUCCESSFULLY =====\n";
        Logger::instance().info("Smart Pantry finished successfully.");
    }
    catch (const AppError& e) {
        Logger::instance().error(std::string("App error: ") + e.what());
        std::cerr << "App error [" << e.code() << "]: " << e.what() << '\n';
        return 1;
    }
    catch (const std::exception& e) {
        Logger::instance().error(std::string("Error: ") + e.what());
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

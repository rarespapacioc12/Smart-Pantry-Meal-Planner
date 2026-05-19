#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include "include/core/Ingredient.hpp"
#include "include/core/Recipe.hpp"
#include "include/core/RecipeBook.hpp"
#include "include/core/Inventory.hpp"
#include "include/core/StockItem.hpp"
#include "include/core/ShoppingItem.hpp"
#include "include/core/StandardShoppingItem.hpp"
#include "include/core/BulkShoppingItem.hpp"
#include "include/core/PromotedShoppingItem.hpp"
#include "include/core/OrganicShoppingItem.hpp"
#include "include/core/SeasonalShoppingItem.hpp"
#include "include/core/ShoppingList.hpp"
#include "include/io/DomainJson.hpp"
#include "include/io/JsonReader.hpp"
#include "include/io/JsonWriter.hpp"
#include "include/utils/Date.hpp"
#include "include/exceptions/AppError.hpp"
#include "include/exceptions/InvalidQuantityError.hpp"
#include "include/exceptions/ItemNotFoundError.hpp"
#include "include/exceptions/PricingCalculationError.hpp"
#include "include/exceptions/JsonParseError.hpp"

namespace {
    const std::string kInventoryIn   = "data/inventory.json";
    const std::string kRecipesIn     = "data/recipes.json";
    const std::string kShoppingIn    = "data/shopping_list.json";
    const std::string kInventoryOut  = "data/inventory.out.json";
    const std::string kRecipesOut    = "data/recipes.out.json";
    const std::string kShoppingOut   = "data/shopping_list.out.json";
}

static void demoInventoryAndRecipesFromJson(){
    std::cout << "===== PANTRY & RECIPES (loaded from JSON) =====\n\n";

    std::cout << "Loading " << kInventoryIn << " ...\n";
    Inventory pantry = DomainJson::loadInventory(kInventoryIn);
    std::cout << "Loading " << kRecipesIn   << " ...\n";
    RecipeBook book  = DomainJson::loadRecipeBook(kRecipesIn);
    std::cout << "Loaded " << pantry.items().size() << " stock items and "
              << book.all().size() << " recipes.\n\n";

    std::cout << "--- Inventory ---\n" << pantry << '\n';

    std::cout << "--- Recipes ---\n" << book << '\n';

    const Date today(2026, 5, 18);
    std::cout << "--- Items expiring within 20 days of " << today.toISO() << " ---\n";
    for(const StockItem& item : pantry.expiringSoon(20, today)){
        std::cout << item;
    }
    std::cout << '\n';

    std::cout << "--- Top recipe recommendations ---\n";
    std::vector<const Recipe*> recommendations = book.recommendTopK(pantry, 3);
    for(std::size_t i = 0; i < recommendations.size(); ++i){
        std::cout << (i + 1) << ". " << recommendations[i]->title() << '\n';
    }
    std::cout << '\n';

    std::cout << "Consuming 200g of Bacon...\n";
    pantry.consumeByExpiry("Bacon", 200);
    std::cout << "Remaining Bacon: " << pantry.available("Bacon") << "g\n\n";

    std::cout << "Saving updated inventory to " << kInventoryOut << " ...\n";
    DomainJson::saveInventory(kInventoryOut, pantry);
    std::cout << "Saving recipes (round-trip) to " << kRecipesOut << " ...\n";
    DomainJson::saveRecipeBook(kRecipesOut, book);
    std::cout << "Done.\n\n";
}

static void demoShoppingListFromJson(){
    std::cout << "===== SHOPPING LIST (loaded from JSON) =====\n\n";

    std::cout << "Loading " << kShoppingIn << " ...\n";
    ShoppingList list = DomainJson::loadShoppingList(kShoppingIn);
    std::cout << "Loaded " << list.size() << " items.\n\n";

    std::cout << list << '\n';

    std::cout << "Total cost: "         << list.calculateTotalCost()             << " lei\n";
    std::cout << "Organic subtotal: "   << list.calculateByCategory("Organic")   << " lei\n";
    std::cout << "Seasonal subtotal: "  << list.calculateByCategory("Seasonal")  << " lei\n\n";

    std::cout << "--- canDeliver() via base pointer ---\n";
    for(const auto& item : list.getPriorityItems(Priority::Low)){
        std::cout << "  " << item->item_name() << " (" << item->category() << "): "
                  << (item->canDeliver() ? "deliverable" : "NOT deliverable") << '\n';
    }
    std::cout << '\n';

    std::cout << "--- Special offers (dynamic_pointer_cast diagnostics) ---\n";
    list.applySpecialOffer(5.0);
    std::cout << '\n';

    std::cout << "Removing 'Pasta' from the list...\n";
    list.removeItem("Pasta");
    std::cout << "Items remaining: " << list.size() << "\n\n";

    std::cout << "Saving updated shopping list to " << kShoppingOut << " ...\n";
    DomainJson::saveShoppingList(kShoppingOut, list);

    std::cout << "Round-trip check: reloading " << kShoppingOut << " ...\n";
    const ShoppingList reloaded = DomainJson::loadShoppingList(kShoppingOut);
    const double original_total = list.calculateTotalCost();
    const double reloaded_total = reloaded.calculateTotalCost();
    std::cout << "  original total = " << original_total
              << " | reloaded total = " << reloaded_total
              << " | match: " << (std::abs(original_total - reloaded_total) < 1e-9 ? "YES" : "NO") << '\n';
    std::cout << "Done.\n\n";
}

static void demoExceptions(){
    std::cout << "===== EXCEPTION HIERARCHY DEMO =====\n\n";

    // 1) InvalidQuantityError thrown from a constructor.
    try {
        StandardShoppingItem bad("Bread", -1, Unit::pcs, 2.0, Priority::Normal, "Local Bakery");
    } catch (const InvalidQuantityError& e){
        std::cout << "Caught InvalidQuantityError: " << e.what()
                  << " [provided=" << e.provided_quantity() << ", code=" << e.code() << "]\n";
    }

    // 2) PricingCalculationError thrown from a constructor.
    try {
        BulkShoppingItem bad("Sugar", 10, Unit::g, -5, Priority::Low, 1, 5);
    } catch (const PricingCalculationError& e){
        std::cout << "Caught PricingCalculationError: " << e.what()
                  << " [category=" << e.category() << ", code=" << e.code() << "]\n";
    }

    // 3) ItemNotFoundError thrown from the static factory.
    try {
        auto item = ShoppingList::createItem("UnknownType", "X", 1, Unit::g, 1, Priority::Low, 0);
        (void)item;
    } catch (const ItemNotFoundError& e){
        std::cout << "Caught ItemNotFoundError: " << e.what()
                  << " [missing=" << e.item_name() << ", code=" << e.code() << "]\n";
    }

    // 4) JsonParseError thrown from the JSON reader.
    try {
        (void)JsonReader::parse("{ \"items\": [1, 2, , 3] }");
    } catch (const JsonParseError& e){
        std::cout << "Caught JsonParseError: " << e.what()
                  << " [line=" << e.line() << ", col=" << e.column() << ", code=" << e.code() << "]\n";
    }

    // 5) Catch as base AppError to show the hierarchy.
    try {
        StandardShoppingItem missing_supplier("Milk", 1, Unit::ml, 5.0, Priority::Normal, "");
    } catch (const AppError& e){
        std::cout << "Caught AppError: " << e.what() << " [code=" << e.code() << "]\n";
    }

    std::cout << '\n';
}

int main() {
    try {
        demoInventoryAndRecipesFromJson();
        demoShoppingListFromJson();
        demoExceptions();
        std::cout << "===== DEMO COMPLETED SUCCESSFULLY =====\n";
    }
    catch(const AppError& e){
        std::cerr << "App error [" << e.code() << "]: " << e.what() << '\n';
        return 1;
    }
    catch(const std::exception& e){
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

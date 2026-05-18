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
#include "include/utils/Date.hpp"
#include "include/exceptions/AppError.hpp"
#include "include/exceptions/InvalidQuantityError.hpp"
#include "include/exceptions/ItemNotFoundError.hpp"
#include "include/exceptions/PricingCalculationError.hpp"

static void demoInventoryAndRecipes(){
    std::cout << "===== PANTRY & RECIPES DEMO =====\n\n";

    Ingredient pasta("Pasta", Unit::g);
    Ingredient eggs("Eggs", Unit::pcs);
    Ingredient bacon("Bacon", Unit::g);
    Ingredient parmesan("Parmesan", Unit::g);
    Ingredient tomatoes("Tomatoes", Unit::g);
    Ingredient olive_oil("Olive Oil", Unit::ml);
    Ingredient salt("Salt", Unit::g);

    std::vector<RecipeLine> carbonara_lines = {
        RecipeLine(pasta, 400, false),
        RecipeLine(eggs, 3, false),
        RecipeLine(bacon, 200, false),
        RecipeLine(parmesan, 100, false),
        RecipeLine(salt, 10, true),
    };
    std::vector<std::string> carbonara_steps = {
        "Boil pasta until al dente",
        "Fry bacon until crispy",
        "Mix eggs with grated parmesan",
        "Combine pasta with bacon",
        "Mix in egg mixture while hot",
    };
    Recipe carbonara("Pasta Carbonara", 20, carbonara_lines, carbonara_steps);

    std::vector<RecipeLine> tomato_pasta_lines = {
        RecipeLine(pasta, 350, false),
        RecipeLine(tomatoes, 500, false),
        RecipeLine(olive_oil, 50, false),
        RecipeLine(salt, 10, true),
    };
    std::vector<std::string> tomato_pasta_steps = {
        "Crush tomatoes",
        "Heat olive oil in pan",
        "Add tomatoes and simmer 15 minutes",
        "Cook pasta separately",
        "Combine with sauce",
    };
    Recipe tomato_pasta("Pasta Pomodoro", 25, tomato_pasta_lines, tomato_pasta_steps);

    RecipeBook recipe_book;
    recipe_book.add(carbonara);
    recipe_book.add(tomato_pasta);
    std::cout << recipe_book << '\n';

    Inventory pantry;
    const Date today(2026, 3, 16);
    pantry.add(StockItem(pasta, 800, Date(2026, 9, 16), Location::Pantry));
    pantry.add(StockItem(eggs, 12, Date(2026, 4, 1), Location::Fridge));
    pantry.add(StockItem(bacon, 300, Date(2026, 3, 25), Location::Fridge));
    pantry.add(StockItem(parmesan, 250, Date(2026, 6, 16), Location::Fridge));
    pantry.add(StockItem(tomatoes, 1000, Date(2026, 3, 30), Location::Pantry));
    pantry.add(StockItem(olive_oil, 500, Date(2027, 3, 16), Location::Pantry));
    pantry.add(StockItem(salt, 500, Date(2027, 12, 31), Location::Pantry));

    std::cout << "--- Full Inventory ---\n" << pantry << '\n';

    std::cout << "--- Top recipe recommendations ---\n";
    const std::vector<const Recipe*> recommendations = recipe_book.recommendTopK(pantry, 2);
    for(std::size_t i = 0; i < recommendations.size(); ++i){
        std::cout << (i + 1) << ". " << recommendations[i]->title() << '\n';
    }
    std::cout << '\n';

    std::cout << "--- Items expiring within 20 days ---\n";
    const std::vector<StockItem> expiring = pantry.expiringSoon(20, today);
    for(const StockItem& item : expiring){
        std::cout << item;
    }
    std::cout << '\n';

    std::cout << "Consuming 200g of Bacon...\n";
    pantry.consumeByExpiry("Bacon", 200);
    std::cout << "Remaining Bacon: " << pantry.available("Bacon") << "g\n\n";
}

static void demoShoppingHierarchy(){
    std::cout << "===== SHOPPING ITEM POLYMORPHISM DEMO =====\n\n";

    ShoppingList list;

    // Build heterogeneous list using the static factory.
    list.addItem(ShoppingList::createItem(
        "Standard", "Pasta", 2, Unit::pcs, 5.5, Priority::Normal, 0,
        Date::today(), 0, Certification::EUOrganic, "Barilla"));
    list.addItem(ShoppingList::createItem(
        "Bulk", "Flour", 10, Unit::g, 3.2, Priority::Medium, 5, Date::today(), 8));
    list.addItem(ShoppingList::createItem(
        "Promoted", "Bacon", 1, Unit::g, 12.0, Priority::High, 25, Date(2030, 12, 31)));
    list.addItem(ShoppingList::createItem(
        "Organic", "Tomatoes", 3, Unit::g, 7.0, Priority::Normal, 20,
        Date::today(), 0, Certification::Bioland));
    // Seasonal: in-season -> 15% off; out-of-season -> 40% surcharge.
    // Reference date is in May, so a Summer item is out-of-season, a Spring item is in-season.
    list.addItem(ShoppingList::createItem(
        "Seasonal", "Strawberries", 2, Unit::g, 10.0, Priority::Normal, 15,
        Date(2026, 5, 18), 40, Certification::EUOrganic, "Generic", Season::Spring));
    list.addItem(ShoppingList::createItem(
        "Seasonal", "Pumpkin", 1, Unit::g, 8.0, Priority::Low, 15,
        Date(2026, 5, 18), 40, Certification::EUOrganic, "Generic", Season::Autumn));

    std::cout << "--- Shopping list (operator<<) ---\n" << list << '\n';

    std::cout << "--- displayByCategory ---\n";
    list.displayByCategory();
    std::cout << '\n';

    std::cout << "Total cost: " << list.calculateTotalCost() << " lei\n";
    std::cout << "Organic subtotal: " << list.calculateByCategory("Organic") << " lei\n";
    std::cout << "Bulk subtotal: "    << list.calculateByCategory("Bulk")    << " lei\n\n";

    std::cout << "--- High-priority items only ---\n";
    for(const auto& item : list.getPriorityItems(Priority::High)){
        std::cout << "  " << *item << '\n';
    }
    std::cout << '\n';

    std::cout << "--- Undiscounted items (no promo) ---\n";
    for(const auto& item : list.getUndiscountedItems()){
        std::cout << "  " << *item << '\n';
    }
    std::cout << '\n';

    std::cout << "--- Special offer pass (dynamic_cast diagnostics) ---\n";
    list.applySpecialOffer(5.0);
    std::cout << '\n';

    // Polymorphism through base pointer: virtual canDeliver().
    std::cout << "--- canDeliver() via base pointer ---\n";
    std::vector<std::shared_ptr<ShoppingItem> > snapshot = list.getPriorityItems(Priority::Low);
    for(const std::shared_ptr<ShoppingItem>& item : snapshot){
        std::cout << "  " << item->item_name() << " (" << item->category() << "): "
                  << (item->canDeliver() ? "deliverable" : "NOT deliverable") << '\n';
    }
    std::cout << '\n';

    // Copy and swap: copying the list must produce independent items (deep clone).
    std::cout << "--- Copy semantics ---\n";
    std::cout << "Lists alive before copy: " << ShoppingList::getTotalListsCreated() << '\n';
    ShoppingList copy = list;
    std::cout << "Lists alive after copy:  " << ShoppingList::getTotalListsCreated() << '\n';
    std::cout << "Copy contains " << copy.size() << " items\n";

    ShoppingList assigned;
    assigned = list;
    std::cout << "After op=, assigned contains " << assigned.size() << " items\n";
    std::cout << "Lists alive after assignment: " << ShoppingList::getTotalListsCreated() << "\n\n";

    // Remove an item from the original; the copy should be unaffected.
    list.removeItem("Pasta");
    std::cout << "Original after removing 'Pasta': " << list.size() << " items\n";
    std::cout << "Copy still:                       " << copy.size() << " items\n\n";
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

    // 4) Catch as base AppError to show the hierarchy.
    try {
        StandardShoppingItem missing_supplier("Milk", 1, Unit::ml, 5.0, Priority::Normal, "");
    } catch (const AppError& e){
        std::cout << "Caught AppError: " << e.what() << " [code=" << e.code() << "]\n";
    }

    std::cout << '\n';
}

int main() {
    try {
        demoInventoryAndRecipes();
        demoShoppingHierarchy();
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

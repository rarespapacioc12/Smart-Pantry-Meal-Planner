#include "../../include/core/MealPlanner.hpp"
#include "../../include/utils/Logger.hpp"

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>

MealPlanner::MealPlanner(const Inventory& inventory,
                         const RecipeBook& book,
                         const PriceCatalog& prices,
                         const ShoppingItemFactory& factory)
    : _inventory(inventory), _book(book), _prices(prices), _factory(factory) {}

struct IngredientShortage {
    Unit unit = Unit::g;
    double quantity = 0.0;
};

static std::map<std::string, IngredientShortage> aggregateShortages(
        const Inventory& inventory,
        const std::vector<const Recipe*>& recipes,
        int portions) {
    std::map<std::string, IngredientShortage> shortages;
    for (const Recipe* recipe : recipes) {
        for (const RecipeLine& line : recipe->lines()) {
            if (line.optional()) {
                continue;
            }
            const std::string& name = line.ingredient().name();
            const double need = line.amount() * portions;
            const double have = inventory.available(name);
            if (have < need) {
                IngredientShortage& s = shortages[name];
                s.unit = line.ingredient().unit();
                s.quantity += (need - have);
            }
        }
    }
    return shortages;
}

ShoppingList MealPlanner::planShoppingForTopRecipes(int topK, int portions,
                                                    const std::string& supplier) const {
    ShoppingList list;
    const std::vector<const Recipe*> recommended = _book.recommendTopK(_inventory, topK);
    const std::map<std::string, IngredientShortage> shortages =
        aggregateShortages(_inventory, recommended, portions);

    for (const auto& [name, shortage] : shortages) {
        if (!_prices.has(name)) {
            Logger::instance().warn("No price for '" + name + "' - skipped from auto-order.");
            continue;
        }
        JsonValue::Object spec;
        spec.emplace("type",      JsonValue(std::string("Standard")));
        spec.emplace("name",      JsonValue(name));
        spec.emplace("quantity",  JsonValue(shortage.quantity));
        spec.emplace("unit",      JsonValue(toString(shortage.unit)));
        spec.emplace("basePrice", JsonValue(_prices.priceOf(name)));
        spec.emplace("priority",  JsonValue(toString(Priority::Normal)));
        spec.emplace("supplier",  JsonValue(supplier));

        list.addItem(std::shared_ptr<ShoppingItem>(
            _factory.createFromJson(JsonValue(std::move(spec)))));
    }
    return list;
}

void MealPlanner::exportPlan(const std::string& path, int topK, int portions,
                             const std::string& supplier) const {
    const std::vector<const Recipe*> recommended = _book.recommendTopK(_inventory, topK);
    const std::map<std::string, IngredientShortage> shortages =
        aggregateShortages(_inventory, recommended, portions);
    const ShoppingList list = planShoppingForTopRecipes(topK, portions, supplier);

    nlohmann::json plan;
    plan["portions"] = portions;
    plan["currency"] = _prices.currency();

    for (const Recipe* recipe : recommended) {
        plan["recommendedRecipes"].push_back({
            {"title", recipe->title()},
            {"prepMinutes", recipe->prepMinutes()}
        });
    }

    double estimatedTotal = 0.0;
    for (const auto& [name, shortage] : shortages) {
        if (!_prices.has(name)) {
            continue;
        }
        const double unitPrice = _prices.priceOf(name);
        const double lineCost = unitPrice * shortage.quantity;
        estimatedTotal += lineCost;
        plan["shoppingList"].push_back({
            {"ingredient", name},
            {"quantity", shortage.quantity},
            {"unit", toString(shortage.unit)},
            {"unitPrice", unitPrice},
            {"lineCost", lineCost}
        });
    }
    plan["estimatedTotal"] = estimatedTotal;

    std::ofstream out(path);
    out << plan.dump(2);
}

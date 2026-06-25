#pragma once
#include <string>

#include "./Inventory.hpp"
#include "./RecipeBook.hpp"
#include "./PriceCatalog.hpp"
#include "./ShoppingList.hpp"
#include "../factory/ShoppingItemFactory.hpp"

class MealPlanner {
private:
    const Inventory& _inventory;
    const RecipeBook& _book;
    const PriceCatalog& _prices;
    const ShoppingItemFactory& _factory;

public:
    MealPlanner(const Inventory& inventory,
                const RecipeBook& book,
                const PriceCatalog& prices,
                const ShoppingItemFactory& factory);

    ShoppingList planShoppingForTopRecipes(int topK, int portions,
                                           const std::string& supplier) const;

    void exportPlan(const std::string& path, int topK, int portions,
                    const std::string& supplier) const;
};

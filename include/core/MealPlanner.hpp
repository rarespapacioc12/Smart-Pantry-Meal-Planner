#pragma once
#include <string>

#include "./Inventory.hpp"
#include "./RecipeBook.hpp"
#include "./PriceCatalog.hpp"
#include "./ShoppingList.hpp"
#include "../factory/ShoppingItemFactory.hpp"

// ============================================================================
// Tema 3 - Logica de business care leaga toate componentele noi.
// ----------------------------------------------------------------------------
// Pe baza inventarului si a retetelor recomandate, calculeaza ingredientele
// lipsa, le transforma intr-o lista de cumparaturi (preturi din PriceCatalog,
// produse create prin ShoppingItemFactory) si exporta planul in JSON (nlohmann).
// ============================================================================
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

    // Genereaza lista de cumparaturi cu ingredientele lipsa pentru primele
    // `topK` retete recomandate, pentru `portions` portii.
    ShoppingList planShoppingForTopRecipes(int topK, int portions,
                                           const std::string& supplier) const;

    // Exporta planul (retete alese + lista de cumparaturi + cost total) in JSON.
    void exportPlan(const std::string& path, int topK, int portions,
                    const std::string& supplier) const;
};

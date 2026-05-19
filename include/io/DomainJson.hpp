#pragma once
#include <string>
#include "./JsonValue.hpp"
#include "../core/Inventory.hpp"
#include "../core/RecipeBook.hpp"
#include "../core/ShoppingList.hpp"

namespace DomainJson {
    // ---- Inventory ----
    JsonValue toJson(const Inventory&);
    Inventory inventoryFromJson(const JsonValue&);
    Inventory loadInventory(const std::string& path);
    void      saveInventory(const std::string& path, const Inventory&);

    // ---- RecipeBook ----
    JsonValue   toJson(const RecipeBook&);
    RecipeBook  recipeBookFromJson(const JsonValue&);
    RecipeBook  loadRecipeBook(const std::string& path);
    void        saveRecipeBook(const std::string& path, const RecipeBook&);

    // ---- ShoppingList ----
    JsonValue    toJson(const ShoppingList&);
    ShoppingList shoppingListFromJson(const JsonValue&);
    ShoppingList loadShoppingList(const std::string& path);
    void         saveShoppingList(const std::string& path, const ShoppingList&);
}

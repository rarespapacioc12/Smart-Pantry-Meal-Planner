#include <iostream>
#include "include/core/Ingredient.hpp"
#include "include/core/Recipe.hpp"
#include "include/core/RecipeBook.hpp"
#include "include/core/Inventory.hpp"
#include "include/core/StockItem.hpp"
#include "include/utils/Date.hpp"

int main() {
    try {
        std::cout << "===== SMART PANTRY MEAL PLANNER DEMO =====\n\n";

        // ===== PART 1: Create Ingredients =====
        std::cout << "--- Creating Ingredients ---\n";
        Ingredient pasta("Pasta", Unit::g);
        Ingredient eggs("Eggs", Unit::pcs);
        Ingredient bacon("Bacon", Unit::g);
        Ingredient parmesan("Parmesan", Unit::g);
        Ingredient tomatoes("Tomatoes", Unit::g);
        Ingredient olive_oil("Olive Oil", Unit::ml);
        Ingredient salt("Salt", Unit::g);
        std::cout << "Created 7 ingredients\n\n";

        // ===== PART 2: Create Recipes using Composition =====
        std::cout << "--- Creating Recipes with RecipeLines ---\n";
        
        std::vector<RecipeLine> carbonara_lines;
        carbonara_lines.push_back(RecipeLine(pasta, 400, false));
        carbonara_lines.push_back(RecipeLine(eggs, 3, false));
        carbonara_lines.push_back(RecipeLine(bacon, 200, false));
        carbonara_lines.push_back(RecipeLine(parmesan, 100, false));
        carbonara_lines.push_back(RecipeLine(salt, 10, true));

        std::vector<std::string> carbonara_steps;
        carbonara_steps.push_back("Boil pasta until al dente");
        carbonara_steps.push_back("Fry bacon until crispy");
        carbonara_steps.push_back("Mix eggs with grated parmesan");
        carbonara_steps.push_back("Combine pasta with bacon");
        carbonara_steps.push_back("Mix in egg mixture while hot");

        Recipe carbonara("Pasta Carbonara", 20, carbonara_lines, carbonara_steps);
        
        std::vector<RecipeLine> tomato_pasta_lines;
        tomato_pasta_lines.push_back(RecipeLine(pasta, 350, false));
        tomato_pasta_lines.push_back(RecipeLine(tomatoes, 500, false));
        tomato_pasta_lines.push_back(RecipeLine(olive_oil, 50, false));
        tomato_pasta_lines.push_back(RecipeLine(salt, 10, true));

        std::vector<std::string> tomato_pasta_steps;
        tomato_pasta_steps.push_back("Crush tomatoes");
        tomato_pasta_steps.push_back("Heat olive oil in pan");
        tomato_pasta_steps.push_back("Add tomatoes and simmer 15 minutes");
        tomato_pasta_steps.push_back("Cook pasta separately");
        tomato_pasta_steps.push_back("Combine with sauce");

        Recipe tomato_pasta("Pasta Pomodoro", 25, tomato_pasta_lines, tomato_pasta_steps);
        std::cout << "Created 2 recipes\n\n";

        // ===== PART 3: Create RecipeBook and add recipes =====
        std::cout << "--- Displaying RecipeBook ---\n";
        RecipeBook recipe_book;
        recipe_book.add(carbonara);
        recipe_book.add(tomato_pasta);
        std::cout << recipe_book << "\n";

        // ===== PART 4: Create Inventory with StockItems (Composition) =====
        std::cout << "--- Creating Inventory with StockItems ---\n";
        Inventory pantry;
        
        // Current date for testing
        Date today(2026, 3, 16);
        
        // Add stock items
        pantry.add(StockItem(pasta, 800, Date(2026, 9, 16), Location::Pantry));
        pantry.add(StockItem(eggs, 12, Date(2026, 4, 1), Location::Fridge));
        pantry.add(StockItem(bacon, 300, Date(2026, 3, 25), Location::Fridge));
        pantry.add(StockItem(parmesan, 250, Date(2026, 6, 16), Location::Fridge));
        pantry.add(StockItem(tomatoes, 1000, Date(2026, 3, 30), Location::Pantry));
        pantry.add(StockItem(olive_oil, 500, Date(2027, 3, 16), Location::Pantry));
        pantry.add(StockItem(salt, 500, Date(2027, 12, 31), Location::Pantry));
        
        std::cout << "Added 7 stock items to inventory\n\n";

        // ===== PART 5: Display full inventory =====
        std::cout << "--- Full Inventory Contents ---\n";
        std::cout << pantry << "\n";

        // ===== PART 6: Test complex functions =====
        std::cout << "--- Testing Complex Functions ---\n";
        
        // Test: Check available quantities
        std::cout << "Available quantities:\n";
        std::cout << "  Pasta: " << pantry.available("Pasta") << "g\n";
        std::cout << "  Eggs: " << pantry.available("Eggs") << " pcs\n";
        std::cout << "  Bacon: " << pantry.available("Bacon") << "g\n\n";
        
        // Test: Find recipe by title
        const Recipe* found_recipe = recipe_book.findByTitle("Pasta Carbonara");
        if(found_recipe) {
            std::cout << "Found recipe: " << found_recipe->title() << "\n";
            std::cout << "Preparation time: " << found_recipe->prepMinutes() << " minutes\n\n";
        }

        // Test: Get top recipe recommendations based on inventory
        std::cout << "--- Top Recipe Recommendations ---\n";
        std::vector<const Recipe*> recommendations = recipe_book.recommendTopK(pantry, 2);
        for(int i = 0; i < (int)recommendations.size(); ++i) {
            std::cout << (i + 1) << ". " << recommendations[i]->title() << "\n";
        }
        std::cout << "\n";

        // Test: Check expiring items
        std::cout << "--- Items Expiring Soon (within 20 days) ---\n";
        std::vector<StockItem> expiring = pantry.expiringSoon(20, today);
        for(const StockItem& item : expiring) {
            std::cout << item << "\n";
        }
        std::cout << "\n";

        // Test: Consume items
        std::cout << "--- Consuming Items (Pasta Carbonara scenario) ---\n";
        std::cout << "Before consuming:\n";
        std::cout << "  Pasta: " << pantry.available("Pasta") << "g\n";
        std::cout << "  Eggs: " << pantry.available("Eggs") << " pcs\n";
        std::cout << "  Bacon: " << pantry.available("Bacon") << "g\n\n";
        
        pantry.consumeByExpiry("Bacon", 200);
        
        std::cout << "After consuming 200g Bacon:\n";
        std::cout << "  Bacon: " << pantry.available("Bacon") << "g\n\n";

        // Test: Remove items
        std::cout << "--- Removing Salt by Name ---\n";
        bool removed = pantry.removeFirstByName("Salt");
        std::cout << "Salt removed: " << (removed ? "yes" : "no") << "\n";
        std::cout << "Remaining salt: " << pantry.available("Salt") << "g\n\n";

        // Test: Test copy constructor and assignment operator
        std::cout << "--- Testing Inventory Copy Constructor and Assignment ---\n";
        Inventory pantry_backup = pantry;  // Copy constructor
        Inventory pantry_restore;
        pantry_restore = pantry;  // Assignment operator
        std::cout << "Created backup and restore copies of inventory\n";
        std::cout << "Backup has " << pantry_backup.items().size() << " items\n";
        std::cout << "Restore has " << pantry_restore.items().size() << " items\n\n";

        std::cout << "===== DEMO COMPLETED SUCCESSFULLY =====\n";
    }
    catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

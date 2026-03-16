#include "../../include/core/RecipeBook.hpp"
#include <vector>
#include <algorithm>

double RecipeBook::scoreRecipe(const Recipe& recipe, const Inventory& inventory) const{
    std::vector<RecipeLine> missing_items = recipe.missing(inventory);
    double score = 0;
    for(const RecipeLine& recipeLine : missing_items){
        score += recipeLine.amount() - inventory.available(recipeLine.ingredient().name());
    }
    return score;
}

RecipeBook::RecipeBook() = default;

RecipeBook::RecipeBook(const std::vector<Recipe>& other){
    this->recipes_ = other;
}

void RecipeBook::add(const Recipe& recipe){
    this->recipes_.push_back(recipe);
}

const Recipe* RecipeBook::findByTitle(const std::string& title) const{
    for(const Recipe& recipe : this->recipes_){
        if(recipe.title() == title){
            return &recipe;
        }
    }
    return nullptr;
}

const std::vector<Recipe>& RecipeBook::all() const{
    return this->recipes_;
}

std::vector<const Recipe*> RecipeBook::recommendTopK(const Inventory& inventory, int k) const{
    std::vector<const Recipe*> recommendations;
    
    std::vector<int> indices;
    for(int i = 0; i < (int)this->recipes_.size(); ++i){
        indices.push_back(i);
    }
    
    std::sort(indices.begin(), indices.end(), [this, inventory](int a, int b){
        return this->scoreRecipe(this->recipes_[a], inventory) < this->scoreRecipe(this->recipes_[b], inventory);
    });
    
    int limit = std::min(k, (int)indices.size());
    for(int i = 0; i < limit; ++i){
        recommendations.push_back(&this->recipes_[indices[i]]);
    }

    return recommendations;
}

std::ostream& operator<<(std::ostream& os, const RecipeBook& recipeBook){
    os << "This recipe book has the following recipes:\n\n";
    for(int i = 0; i < (int)recipeBook.recipes_.size(); ++i){
        os << std::to_string(i + 1) << ". " << recipeBook.recipes_[i];
        os << '\n';
    }
    return os;
}
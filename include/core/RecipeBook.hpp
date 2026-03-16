#pragma once
#include "./Recipe.hpp"
#include <vector>

class RecipeBook{
private:
    std::vector<Recipe> recipes_;
    double scoreRecipe(const Recipe&, const Inventory&) const;

public:
    RecipeBook();
    RecipeBook(const std::vector<Recipe>&);
    void add(const Recipe&);
    const Recipe* findByTitle(const std::string&) const;
    const std::vector<Recipe>& all() const;
    std::vector<const Recipe*> recommendTopK(const Inventory&, int) const;
    friend std::ostream& operator<<(std::ostream&, const RecipeBook&);
};
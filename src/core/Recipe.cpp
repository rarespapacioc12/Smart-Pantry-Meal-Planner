#include "../../include/core/Recipe.hpp"
#include <vector>
#include <string>
#include <stdexcept>

Recipe::Recipe(const std::string& title, int prepMinutes, const std::vector<RecipeLine>& lines, const std::vector<std::string>& steps){
    if(prepMinutes < 0){
        throw std::runtime_error("Prep minutes may not be negative");
    }
    if(lines.empty()){
        throw std::runtime_error("Recipe line may not be empty");
    }

    this->title_ = title;
    this->prepMinutes_ = prepMinutes;
    this->lines_ = lines;
    this->steps_ = steps;
}

const std::string& Recipe::title() const{
    return this->title_;
}

int Recipe::prepMinutes() const{
    return this->prepMinutes_;
}

const std::vector<RecipeLine>& Recipe::lines() const{
    return this->lines_;
}

const std::vector<std::string>& Recipe::steps() const{
    return this->steps_;
}

Recipe Recipe::scaled(int multiplier) const{
    Recipe recipe(this->title_, this->prepMinutes_, {}, this->steps_);

    for(const RecipeLine& recipeLine : this->lines_){
        recipe.lines_.push_back(RecipeLine(recipeLine.ingredient(), recipeLine.amount() * multiplier, recipeLine.optional()));
    }

    return recipe;
}

std::vector<RecipeLine> Recipe::missing(const Inventory& inventory, int portions) const{
    std::vector<RecipeLine> missing_items;
    
    for(const RecipeLine& recipeLine : this->lines_){
        double need = recipeLine.amount() * portions;
        double have = inventory.available(recipeLine.ingredient().name());
        if(have < need && !recipeLine.optional()){
            missing_items.push_back(recipeLine);
        }
    }

    return missing_items;
}

std::ostream& operator<<(std::ostream& os, const Recipe& recipe){
    os << "---------- " << recipe.title() << " ----------\n\n";
    os << "Preparation time: " << recipe.prepMinutes() << "\n\n";
    os << "Necessary ingredients:\n\n";
    for(const RecipeLine& recipeLine : recipe.lines()){
        os << recipeLine;
    }
    os << '\n';
    os << "The steps that you should follow in this recipe are:\n";
    for(int i = 0; i < (int)recipe.steps().size(); ++i){
        os << (i + 1) << ". " << recipe.steps()[i] << '\n';
    }
    return os;
}
#pragma once
#include "./RecipeLine.hpp"
#include "./Inventory.hpp"
#include <vector>
#include <string>

class Recipe{
private:
    std::string title_;
    int prepMinutes_;
    std::vector<RecipeLine> lines_;
    std::vector<std::string> steps_;

public:
    Recipe(const std::string& title, int prepMinutes, const std::vector<RecipeLine>& lines, const std::vector<std::string>& steps = {});
    const std::string& title() const;
    int prepMinutes() const;
    const std::vector<RecipeLine>& lines() const;
    const std::vector<std::string>& steps() const;
    Recipe scaled(int) const;
    std::vector<RecipeLine> missing(const Inventory& inventory, int portions = 1) const;
    friend std::ostream& operator<<(std::ostream&, const Recipe&);
};
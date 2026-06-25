#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../core/ShoppingItem.hpp"
#include "../io/JsonValue.hpp"

class ShoppingItemFactory {
public:
    using Creator = std::function<std::unique_ptr<ShoppingItem>(const JsonValue&)>;

    ShoppingItemFactory();

    std::unique_ptr<ShoppingItem> createFromJson(const JsonValue& spec) const;

    std::unique_ptr<ShoppingItem> create(const std::string& type, const JsonValue& spec) const;

    std::vector<std::string> registeredTypes() const;

private:
    std::map<std::string, Creator> _creators;
};

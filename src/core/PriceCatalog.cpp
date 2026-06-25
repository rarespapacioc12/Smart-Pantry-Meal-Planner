#include "../../include/core/PriceCatalog.hpp"
#include "../../include/exceptions/AppError.hpp"
#include "../../include/exceptions/ItemNotFoundError.hpp"

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

void PriceCatalog::load(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw AppError("Cannot open prices file: " + path, "PRICES_IO");
    }

    nlohmann::json j;
    try {
        in >> j;
    } catch (const nlohmann::json::parse_error& e) {
        throw AppError(std::string("Prices parse error: ") + e.what(), "PRICES_PARSE");
    }

    _currency = j.value("currency", std::string("lei"));
    _prices.clear();
    for (const auto& entry : j.at("prices")) {
        _prices[entry.at("ingredient").get<std::string>()] =
            entry.at("pricePerUnit").get<double>();
    }
}

double PriceCatalog::priceOf(const std::string& ingredient) const {
    auto it = _prices.find(ingredient);
    if (it == _prices.end()) {
        throw ItemNotFoundError(ingredient);
    }
    return it->second;
}

bool PriceCatalog::has(const std::string& ingredient) const {
    return _prices.find(ingredient) != _prices.end();
}

const std::string& PriceCatalog::currency() const { return _currency; }

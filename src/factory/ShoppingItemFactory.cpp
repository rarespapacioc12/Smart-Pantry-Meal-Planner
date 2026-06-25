#include "../../include/factory/ShoppingItemFactory.hpp"

#include "../../include/core/StandardShoppingItem.hpp"
#include "../../include/core/PromotedShoppingItem.hpp"
#include "../../include/core/BulkShoppingItem.hpp"
#include "../../include/core/OrganicShoppingItem.hpp"
#include "../../include/core/SeasonalShoppingItem.hpp"
#include "../../include/exceptions/AppError.hpp"
#include "../../include/exceptions/ItemNotFoundError.hpp"

namespace {
    // Campurile comune tuturor produselor.
    struct Common {
        std::string name;
        double quantity;
        Unit unit;
        double basePrice;
        Priority priority;
    };

    Priority parsePriorityName(const std::string& s) {
        if (s == "Low")    return Priority::Low;
        if (s == "Normal") return Priority::Normal;
        if (s == "Medium") return Priority::Medium;
        if (s == "High")   return Priority::High;
        if (s == "Urgent") return Priority::Urgent;
        throw AppError("Unknown priority: " + s, "FACTORY");
    }

    Season parseSeason(const std::string& s) {
        if (s == "Spring") return Season::Spring;
        if (s == "Summer") return Season::Summer;
        if (s == "Autumn") return Season::Autumn;
        if (s == "Winter") return Season::Winter;
        throw AppError("Unknown season: " + s, "FACTORY");
    }

    Common readCommon(const JsonValue& s) {
        return Common{
            s.at("name").asString(),
            s.at("quantity").asNumber(),
            parseUnit(s.at("unit").asString()),
            s.at("basePrice").asNumber(),
            parsePriorityName(s.at("priority").asString())
        };
    }
}  // namespace

ShoppingItemFactory::ShoppingItemFactory() {
    _creators.emplace("Standard", [](const JsonValue& s) -> std::unique_ptr<ShoppingItem> {
        const Common c = readCommon(s);
        return std::make_unique<StandardShoppingItem>(
            c.name, c.quantity, c.unit, c.basePrice, c.priority,
            s.at("supplier").asString());
    });

    _creators.emplace("Promoted", [](const JsonValue& s) -> std::unique_ptr<ShoppingItem> {
        const Common c = readCommon(s);
        const Date end = s.has("promoEnd")
            ? Date::fromISO(s.at("promoEnd").asString())
            : Date(2099, 12, 31);
        return std::make_unique<PromotedShoppingItem>(
            c.name, c.quantity, c.unit, c.basePrice, c.priority,
            s.at("discountPercentage").asNumber(), end);
    });

    _creators.emplace("Bulk", [](const JsonValue& s) -> std::unique_ptr<ShoppingItem> {
        const Common c = readCommon(s);
        return std::make_unique<BulkShoppingItem>(
            c.name, c.quantity, c.unit, c.basePrice, c.priority,
            s.at("minQuantity").asNumber(), s.at("bulkDiscount").asNumber());
    });

    _creators.emplace("Organic", [](const JsonValue& s) -> std::unique_ptr<ShoppingItem> {
        const Common c = readCommon(s);
        return std::make_unique<OrganicShoppingItem>(
            c.name, c.quantity, c.unit, c.basePrice, c.priority,
            parseCertification(s.at("certification").asString()),
            s.at("organicMarkup").asNumber());
    });

    _creators.emplace("Seasonal", [](const JsonValue& s) -> std::unique_ptr<ShoppingItem> {
        const Common c = readCommon(s);
        const Date ref = s.has("referenceDate")
            ? Date::fromISO(s.at("referenceDate").asString())
            : Date::today();
        return std::make_unique<SeasonalShoppingItem>(
            c.name, c.quantity, c.unit, c.basePrice, c.priority,
            parseSeason(s.at("season").asString()),
            s.at("inSeasonDiscount").asNumber(),
            s.at("offSeasonSurcharge").asNumber(),
            ref);
    });
}

std::unique_ptr<ShoppingItem> ShoppingItemFactory::create(const std::string& type,
                                                          const JsonValue& spec) const {
    auto it = _creators.find(type);
    if (it == _creators.end()) {
        throw ItemNotFoundError(type);  // tip de produs neinregistrat
    }
    return it->second(spec);
}

std::unique_ptr<ShoppingItem> ShoppingItemFactory::createFromJson(const JsonValue& spec) const {
    return create(spec.at("type").asString(), spec);
}

std::vector<std::string> ShoppingItemFactory::registeredTypes() const {
    std::vector<std::string> types;
    types.reserve(_creators.size());
    for (const auto& [type, creator] : _creators) {
        (void)creator;
        types.push_back(type);
    }
    return types;
}

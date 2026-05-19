#include "../../include/io/DomainJson.hpp"
#include "../../include/io/JsonReader.hpp"
#include "../../include/io/JsonWriter.hpp"
#include "../../include/core/SeasonalShoppingItem.hpp"
#include "../../include/core/StandardShoppingItem.hpp"
#include "../../include/core/BulkShoppingItem.hpp"
#include "../../include/core/PromotedShoppingItem.hpp"
#include "../../include/core/OrganicShoppingItem.hpp"
#include "../../include/exceptions/AppError.hpp"

namespace {
    Season parseSeason(const std::string& s){
        if(s == "Spring") return Season::Spring;
        if(s == "Summer") return Season::Summer;
        if(s == "Autumn") return Season::Autumn;
        if(s == "Winter") return Season::Winter;
        throw AppError("Unknown season: " + s, "JSON_DOMAIN");
    }
}

namespace DomainJson {

// ---------- Inventory ----------

JsonValue toJson(const Inventory& inv){
    JsonValue::Array items;
    for(const StockItem& s : inv.items()){
        JsonValue::Object o;
        o.emplace("ingredient", JsonValue(s.ingredient().name()));
        o.emplace("unit",       JsonValue(toString(s.ingredient().unit())));
        o.emplace("quantity",   JsonValue(s.quantity()));
        o.emplace("expiry",     JsonValue(s.expiry().toISO()));
        o.emplace("location",   JsonValue(toString(s.location())));
        items.push_back(JsonValue(std::move(o)));
    }
    JsonValue::Object root;
    root.emplace("items", JsonValue(std::move(items)));
    return JsonValue(std::move(root));
}

Inventory inventoryFromJson(const JsonValue& v){
    Inventory inv;
    for(const JsonValue& itemJson : v.at("items").asArray()){
        Ingredient ingredient(
            itemJson.at("ingredient").asString(),
            parseUnit(itemJson.at("unit").asString())
        );
        inv.add(StockItem(
            ingredient,
            itemJson.at("quantity").asNumber(),
            Date::fromISO(itemJson.at("expiry").asString()),
            parseLocation(itemJson.at("location").asString())
        ));
    }
    return inv;
}

Inventory loadInventory(const std::string& path){
    return inventoryFromJson(JsonReader::parseFile(path));
}

void saveInventory(const std::string& path, const Inventory& inv){
    JsonWriter::writeFile(path, toJson(inv));
}

// ---------- RecipeBook ----------

JsonValue toJson(const RecipeBook& book){
    JsonValue::Array recipes;
    for(const Recipe& r : book.all()){
        JsonValue::Array lines;
        for(const RecipeLine& line : r.lines()){
            JsonValue::Object lo;
            lo.emplace("ingredient", JsonValue(line.ingredient().name()));
            lo.emplace("unit",       JsonValue(toString(line.ingredient().unit())));
            lo.emplace("amount",     JsonValue(line.amount()));
            lo.emplace("optional",   JsonValue(line.optional()));
            lines.push_back(JsonValue(std::move(lo)));
        }
        JsonValue::Array steps;
        for(const std::string& step : r.steps()){
            steps.push_back(JsonValue(step));
        }
        JsonValue::Object recipe;
        recipe.emplace("title",       JsonValue(r.title()));
        recipe.emplace("prepMinutes", JsonValue(r.prepMinutes()));
        recipe.emplace("lines",       JsonValue(std::move(lines)));
        recipe.emplace("steps",       JsonValue(std::move(steps)));
        recipes.push_back(JsonValue(std::move(recipe)));
    }
    JsonValue::Object root;
    root.emplace("recipes", JsonValue(std::move(recipes)));
    return JsonValue(std::move(root));
}

RecipeBook recipeBookFromJson(const JsonValue& v){
    RecipeBook book;
    for(const JsonValue& r : v.at("recipes").asArray()){
        std::vector<RecipeLine> lines;
        for(const JsonValue& line : r.at("lines").asArray()){
            Ingredient ingredient(
                line.at("ingredient").asString(),
                parseUnit(line.at("unit").asString())
            );
            lines.emplace_back(
                ingredient,
                line.at("amount").asNumber(),
                line.at("optional").asBool()
            );
        }
        std::vector<std::string> steps;
        if(r.has("steps")){
            for(const JsonValue& step : r.at("steps").asArray()){
                steps.push_back(step.asString());
            }
        }
        book.add(Recipe(
            r.at("title").asString(),
            static_cast<int>(r.at("prepMinutes").asNumber()),
            lines,
            steps
        ));
    }
    return book;
}

RecipeBook loadRecipeBook(const std::string& path){
    return recipeBookFromJson(JsonReader::parseFile(path));
}

void saveRecipeBook(const std::string& path, const RecipeBook& book){
    JsonWriter::writeFile(path, toJson(book));
}

// ---------- ShoppingList ----------

namespace {
    JsonValue itemToJson(const ShoppingItem& item){
        JsonValue::Object o;
        o.emplace("type",      JsonValue(item.category()));
        o.emplace("name",      JsonValue(item.item_name()));
        o.emplace("quantity",  JsonValue(item.quantity()));
        o.emplace("unit",      JsonValue(toString(item.unit())));
        o.emplace("basePrice", JsonValue(item.base_price()));
        o.emplace("priority",  JsonValue(toString(item.priority())));

        if(const auto* s = dynamic_cast<const StandardShoppingItem*>(&item)){
            o.emplace("supplier", JsonValue(s->supplier()));
        } else if(const auto* p = dynamic_cast<const PromotedShoppingItem*>(&item)){
            o.emplace("discountPercentage", JsonValue(p->discount_percentage()));
            o.emplace("promoEnd",           JsonValue(p->promo_end().toISO()));
        } else if(const auto* b = dynamic_cast<const BulkShoppingItem*>(&item)){
            o.emplace("minQuantity",  JsonValue(b->min_quantity()));
            o.emplace("bulkDiscount", JsonValue(b->bulk_discount()));
        } else if(const auto* og = dynamic_cast<const OrganicShoppingItem*>(&item)){
            o.emplace("certification", JsonValue(toString(og->certification())));
            o.emplace("organicMarkup", JsonValue(og->organic_markup()));
        } else if(const auto* sn = dynamic_cast<const SeasonalShoppingItem*>(&item)){
            o.emplace("season",             JsonValue(toString(sn->season())));
            o.emplace("inSeasonDiscount",   JsonValue(sn->inSeasonDiscount()));
            o.emplace("offSeasonSurcharge", JsonValue(sn->offSeasonSurcharge()));
            o.emplace("referenceDate",      JsonValue(sn->referenceDate().toISO()));
        }
        return JsonValue(std::move(o));
    }

    Priority parsePriorityName(const std::string& s){
        if(s == "Low")    return Priority::Low;
        if(s == "Normal") return Priority::Normal;
        if(s == "Medium") return Priority::Medium;
        if(s == "High")   return Priority::High;
        if(s == "Urgent") return Priority::Urgent;
        throw AppError("Unknown priority: " + s, "JSON_DOMAIN");
    }
}

JsonValue toJson(const ShoppingList& list){
    JsonValue::Object root;
    root.emplace("createdAt", JsonValue(list.createDate().toISO()));
    JsonValue::Array items;
    for(const auto& item : list.getPriorityItems(Priority::Low)){
        items.push_back(itemToJson(*item));
    }
    root.emplace("items", JsonValue(std::move(items)));
    return JsonValue(std::move(root));
}

ShoppingList shoppingListFromJson(const JsonValue& v){
    ShoppingList list;
    for(const JsonValue& itemJson : v.at("items").asArray()){
        const std::string& type = itemJson.at("type").asString();
        const std::string& name = itemJson.at("name").asString();
        const double       qty  = itemJson.at("quantity").asNumber();
        const Unit         unit = parseUnit(itemJson.at("unit").asString());
        const double       price = itemJson.at("basePrice").asNumber();
        const Priority     pri  = parsePriorityName(itemJson.at("priority").asString());

        if(type == "Standard"){
            list.addItem(ShoppingList::createItem(
                type, name, qty, unit, price, pri, 0,
                Date::today(), 0, Certification::EUOrganic,
                itemJson.at("supplier").asString()));
        } else if(type == "Promoted"){
            const Date end = itemJson.has("promoEnd")
                ? Date::fromISO(itemJson.at("promoEnd").asString())
                : Date(2099, 12, 31);
            list.addItem(ShoppingList::createItem(
                type, name, qty, unit, price, pri,
                itemJson.at("discountPercentage").asNumber(),
                end));
        } else if(type == "Bulk"){
            list.addItem(ShoppingList::createItem(
                type, name, qty, unit, price, pri,
                itemJson.at("minQuantity").asNumber(),
                Date::today(),
                itemJson.at("bulkDiscount").asNumber()));
        } else if(type == "Organic"){
            const std::string& certStr = itemJson.at("certification").asString();
            list.addItem(ShoppingList::createItem(
                type, name, qty, unit, price, pri,
                itemJson.at("organicMarkup").asNumber(),
                Date::today(), 0,
                parseCertification(certStr)));
        } else if(type == "Seasonal"){
            const Date ref = itemJson.has("referenceDate")
                ? Date::fromISO(itemJson.at("referenceDate").asString())
                : Date::today();
            list.addItem(ShoppingList::createItem(
                type, name, qty, unit, price, pri,
                itemJson.at("inSeasonDiscount").asNumber(),
                ref,
                itemJson.at("offSeasonSurcharge").asNumber(),
                Certification::EUOrganic, "Generic",
                parseSeason(itemJson.at("season").asString())));
        } else {
            throw AppError("Unknown shopping item type: " + type, "JSON_DOMAIN");
        }
    }
    return list;
}

ShoppingList loadShoppingList(const std::string& path){
    return shoppingListFromJson(JsonReader::parseFile(path));
}

void saveShoppingList(const std::string& path, const ShoppingList& list){
    JsonWriter::writeFile(path, toJson(list));
}

}  // namespace DomainJson

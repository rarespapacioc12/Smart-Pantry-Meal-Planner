#include "../../include/core/SeasonalShoppingItem.hpp"

namespace {
    int monthOf(const Date& d){
        const std::string iso = d.toISO();
        return (iso[5] - '0') * 10 + (iso[6] - '0');
    }
}

std::string toString(Season s){
    switch(s){
        case Season::Spring: return "Spring";
        case Season::Summer: return "Summer";
        case Season::Autumn: return "Autumn";
        case Season::Winter: return "Winter";
    }
    return "Unknown";
}

bool isInSeason(Season s, int month){
    switch(s){
        case Season::Spring: return month >= 3  && month <= 5;
        case Season::Summer: return month >= 6  && month <= 8;
        case Season::Autumn: return month >= 9  && month <= 11;
        case Season::Winter: return month == 12 || month <= 2;
    }
    return false;
}

SeasonalShoppingItem::SeasonalShoppingItem(const std::string& item_name, double quantity, Unit unit,
double base_price, Priority priority, Season season, double in_season_discount,
double off_season_surcharge, const Date& reference_date)
: ShoppingItem(item_name, quantity, unit, base_price, priority),
  _season(season),
  _off_season_surcharge(off_season_surcharge),
  _in_season_discount(in_season_discount),
  _reference_date(reference_date)
{
    if(in_season_discount < 0 || in_season_discount > 100){
        throw InvalidQuantityError(in_season_discount);
    }
    if(off_season_surcharge < 0 || off_season_surcharge > 200){
        throw InvalidQuantityError(off_season_surcharge);
    }
}

double SeasonalShoppingItem::calculateTotalCost() const {
    const double base = quantity() * base_price();
    return inSeason()
        ? base * (1.0 - _in_season_discount  / 100.0)
        : base * (1.0 + _off_season_surcharge / 100.0);
}

std::string SeasonalShoppingItem::category() const {
    return "Seasonal";
}

SeasonalShoppingItem* SeasonalShoppingItem::clone() const {
    return new SeasonalShoppingItem(*this);
}

std::ostream& SeasonalShoppingItem::print(std::ostream& os) const {
    ShoppingItem::print(os);
    os << " | Season: " << toString(_season)
       << (inSeason() ? " (in-season -" : " (off-season +")
       << (inSeason() ? _in_season_discount : _off_season_surcharge) << "%)";
    return os;
}

bool SeasonalShoppingItem::canDeliver() const {
    return inSeason();
}

Season SeasonalShoppingItem::season() const {
    return _season;
}

bool SeasonalShoppingItem::inSeason() const {
    return isInSeason(_season, monthOf(_reference_date));
}

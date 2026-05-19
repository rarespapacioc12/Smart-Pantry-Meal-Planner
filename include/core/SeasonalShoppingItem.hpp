#pragma once
#include "./ShoppingItem.hpp"
#include "../utils/Date.hpp"
#include "../exceptions/InvalidQuantityError.hpp"

enum class Season { Spring, Summer, Autumn, Winter };

std::string toString(Season);
bool isInSeason(Season, int month);

class SeasonalShoppingItem : public ShoppingItem {
private:
    Season _season;
    double _off_season_surcharge;
    double _in_season_discount;
    Date _reference_date;

public:
    SeasonalShoppingItem(const std::string&, double, Unit, double, Priority,
                         Season, double in_season_discount, double off_season_surcharge,
                         const Date& reference_date = Date::today());

    double calculateTotalCost() const override;
    std::string category() const override;
    SeasonalShoppingItem* clone() const override;
    std::ostream& print(std::ostream&) const override;
    bool canDeliver() const override;

    Season season() const;
    bool inSeason() const;
    double inSeasonDiscount() const;
    double offSeasonSurcharge() const;
    const Date& referenceDate() const;
};

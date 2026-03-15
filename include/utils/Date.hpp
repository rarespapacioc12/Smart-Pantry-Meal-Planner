#pragma once
#include <iostream>
#include <string>

const int ISOFormatLength = 10;
const int FebruaryNormal = 28;
const int FebruaryLeap = 29;
const int Short = 30;
const int Long = 31;
const int NormalYear = 365;
const int LeapYear = 366;

enum class Month{
    January = 1,
    February = 2,
    March = 3,
    April = 4,
    May = 5,
    June = 6,
    July = 7,
    August = 8,
    September = 9,
    October = 10,
    November = 11,
    December = 12
};

Month parseInt(int);
int toInt(const Month&);
int countDays(int, int);
int countDays(int, int, bool);

int getNumberOfDays(const Month&, bool);
bool isLeapYear(int);

class Date{
private:
    int year = 0;
    int month = 0;
    int day = 0;

public:
    Date(int, int, int);
    static Date fromISO(const std::string&);
    std::string toISO() const;
    bool operator<(const Date&) const;
    bool operator==(const Date&) const;
    int daysUntil(const Date&) const;
    friend std::ostream& operator<<(std::ostream&, const Date&);
};


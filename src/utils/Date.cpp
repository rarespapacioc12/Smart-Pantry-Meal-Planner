#include "../../include/utils/Date.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

int getNumberOfDays(const Month& month, bool leapYear){
    switch(month){
        case Month::January: case Month::March: case Month::May: case Month::July: case Month::August: case Month::October: case Month::December:
            return Long;
            break;
        case Month::April: case Month::June: case Month::September: case Month::November:
            return Short;
            break;
        case Month::February:
            return (leapYear ? FebruaryLeap : FebruaryNormal);
            break;
    }
    throw std::runtime_error("Invalid month: " + std::to_string(toInt(month)) + "\n");
}

Month parseInt(int month){
    switch(month){
        case 1:
            return Month::January;
        case 2:
            return Month::February;
        case 3:
            return Month::March;
        case 4:
            return Month::April;
        case 5:
            return Month::May;
        case 6:
            return Month::June;
        case 7:
            return Month::July;
        case 8:
            return Month::August;
        case 9:
            return Month::September;
        case 10:
            return Month::October;
        case 11:
            return Month::November;
        case 12:
            return Month::December;
    }
    throw std::runtime_error("Invalid month: " + std::to_string(month) + "\n");
}

int toInt(const Month& month){
    switch(month){
        case Month::January:
            return 1;
        case Month::February:
            return 2;
        case Month::March:
            return 3;
        case Month::April:
            return 4;
        case Month::May:
            return 5;
        case Month::June:
            return 6;
        case Month::July:
            return 7;
        case Month::August:
            return 8;
        case Month::September:
            return 9;
        case Month::October:
            return 10;
        case Month::November:
            return 11;
        case Month::December:
            return 12;
    }
    throw std::runtime_error("Invalid month: " + std::to_string(toInt(month)) + "\n");
}

int countDays(int start, int stop, bool leapYear){
    int counter = 0;
    for(int i = start; i <= stop; ++i){
        counter += getNumberOfDays(parseInt(i), leapYear);
    }
    return counter;
}

int countDays(int yearStart, int yearStop){
    int counter = 0;
    for(int i = yearStart; i <= yearStop; ++i)
        counter += (isLeapYear(i) ? LeapYear : NormalYear);
    return counter;
}

bool isLeapYear(int year){
    if(!(year % 4 == 0) || (year % 100 == 0 && year % 400 != 0))
        return false;
    return true;
}

Date::Date() = default;

Date::Date(int year, int month, int day){
    if(year <= 0){ // Consideram ca ne intereseaza datele dupa Hristos
        throw std::runtime_error("Invalid year: " + std::to_string(year) + "\n");
    }
    if(month < 1 || month > 12){ // Se cunoaste ca lunile sunt intre 1 si 12
        throw std::runtime_error("Invalid month: " + std::to_string(month) + "\n");
    }
    if(day < 1 || day > getNumberOfDays(parseInt(month), isLeapYear(year))){ // Este evident ca o zi valida este din intervalul [1, NrMaximZileLuna]
        throw std::runtime_error("Invalid day: " + std::to_string(day) + "\n");
    }
    this->year = year;
    this->month = month;
    this->day = day;
}

// Format ISO
Date Date::fromISO(const std::string& date){
    if((int)date.length() != 10 || date[4] != '-' || date[7] != '-'){
        throw std::runtime_error("Invalid format");
    }

    int year = 0, month = 0, day = 0;

    for(int i = 0; i < 4; ++i){
        if(!std::isdigit(date[i])){
            throw std::runtime_error("Invalid format");
        }
        year = year * 10 + (date[i] - '0');
    }
        
    for(int i = 5; i < 7; ++i){
        if(!std::isdigit(date[i])){
            throw std::runtime_error("Invalid format");
        }
        month = month * 10 + (date[i] - '0');
    }
        
    for(int i = 8; i < 10; ++i){
        if(!std::isdigit(date[i])){
            throw std::runtime_error("Invalid format");
        }
        day = day * 10 + (date[i] - '0');
    }
        
    return Date(year, month, day);
}

std::string Date::toISO() const {
    return std::to_string(this->year) 
    + "-"
    + (this->month < 10 ? "0" : "") 
    + std::to_string(this->month) 
    + "-"
    + (this->day < 10 ? "0" : "") 
    + std::to_string(this->day);
}

bool Date::operator<(const Date& other) const{
    if(this->year != other.year) { return this->year < other.year; }
    if(this->month != other.month) { return this->month < other.month; }
    if(this->day != other.day) { return this->day < other.day; }
    return false; // Daca avem doua numere egale, nu poti spune ca 1 < 1.
}

bool Date::operator==(const Date& other) const{
    return (this->year == other.year && this->month == other.month && this->day == other.day);
}

int Date::daysUntil(const Date& other) const{
    if(other < *this){
        throw std::runtime_error("Date already passed");
    }

    if(this->year == other.year){
        if(this->month == other.month){
            return other.day - this->day;
        }
        else{ // Implicit din verificarea de la data avem this->month < other.month
            if(this->day <= other.day){
                return (other.day - this->day)
                + countDays(this->month, other.month - 1, isLeapYear(this->year));
            }
            else{
                return other.day 
                + getNumberOfDays(parseInt(this->month), isLeapYear(this->year)) - this->day
                + countDays(this->month + 1, other.month - 1, isLeapYear(this->year));
            }
        }
    }
    else{
        if(this->day <= other.day){
            return (other.day - this->day)
            + countDays(this->month, toInt(Month::December), isLeapYear(this->year))
            + countDays(toInt(Month::January), other.month, isLeapYear(other.year))
            + countDays(this->year + 1, other.year - 1);
        }
        else{
            return (getNumberOfDays(parseInt(this->month), isLeapYear(this->year)) - this->day)
            + other.day
            + countDays(this->month + 1, toInt(Month::December), isLeapYear(this->year))
            + countDays(toInt(Month::January), other.month - 1, isLeapYear(other.year))
            + countDays(this->year + 1, other.year - 1);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Date& date){
    os << date.toISO() << '\n';
    return os;
}
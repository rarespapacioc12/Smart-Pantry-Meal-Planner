#pragma once
#include "./AppError.hpp"

class ItemNotFoundError : public AppError{
private:
    std::string _item_name;
public:
    ItemNotFoundError(const std::string&);
    const std::string& item_name() const;
};

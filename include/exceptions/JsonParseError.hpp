#pragma once
#include "./AppError.hpp"

class JsonParseError : public AppError {
private:
    std::size_t _line;
    std::size_t _column;
public:
    JsonParseError(const std::string& reason, std::size_t line, std::size_t column);
    std::size_t line() const;
    std::size_t column() const;
};

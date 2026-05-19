#include "../../include/exceptions/JsonParseError.hpp"

JsonParseError::JsonParseError(const std::string& reason, std::size_t line, std::size_t column)
: AppError("JSON parse error at line " + std::to_string(line) + ", column " + std::to_string(column) + ": " + reason, "JSON_PARSE"),
  _line(line),
  _column(column) {}

std::size_t JsonParseError::line() const { return _line; }
std::size_t JsonParseError::column() const { return _column; }

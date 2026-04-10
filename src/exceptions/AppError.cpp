#include "../../include/exceptions/AppError.hpp"

AppError::AppError(const std::string& message, const std::string& error_code)
: _message(message), _error_code(error_code) {}

const char* AppError::what() const noexcept{
    return _message.c_str();
}

const std::string& AppError::code() const{
    return _error_code;
}

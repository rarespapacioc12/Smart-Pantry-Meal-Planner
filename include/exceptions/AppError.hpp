#pragma once
#include <exception>
#include <string>

class AppError : public std::exception{
private:
    std::string _message;
    std::string _error_code;
public:
    AppError(const std::string&, const std::string&);
    virtual ~AppError() noexcept = default;
    const char* what() const noexcept override;
    const std::string& code() const;
};

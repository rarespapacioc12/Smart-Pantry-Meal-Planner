#pragma once
#include <memory>
#include <string>

namespace spdlog { class logger; }

class Logger {
public:
    static Logger& instance();

    void init(const std::string& level, const std::string& file);

    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger();
    ~Logger();

    std::shared_ptr<spdlog::logger> _logger;
};

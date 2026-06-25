#include "../../include/utils/Logger.hpp"

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

Logger& Logger::instance() {
    static Logger s_instance;
    return s_instance;
}

Logger::Logger() = default;
Logger::~Logger() = default;

void Logger::init(const std::string& level, const std::string& file) {
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file, true);

    _logger = std::make_shared<spdlog::logger>(
        "smartpantry", spdlog::sinks_init_list{console, file_sink});
    _logger->set_level(spdlog::level::from_str(level));
    _logger->flush_on(spdlog::level::warn);
    _logger->set_pattern("[%H:%M:%S] [%^%l%$] %v");
}

void Logger::info(const std::string& message) {
    if (_logger) {
        _logger->info(message);
    }
}

void Logger::warn(const std::string& message) {
    if (_logger) {
        _logger->warn(message);
    }
}

void Logger::error(const std::string& message) {
    if (_logger) {
        _logger->error(message);
    }
}

#pragma once
#include <memory>
#include <string>

// Forward declaration ca sa nu expunem antetele spdlog in tot proiectul.
namespace spdlog { class logger; }

// ============================================================================
// Tema 3 - DESIGN PATTERN: Singleton (+ biblioteca externa spdlog)
// ----------------------------------------------------------------------------
// Punct unic de logare pentru intreaga aplicatie. Inlocuieste o eventuala
// variabila globala de logging si garanteaza o singura instanta a logger-ului.
// Wrapper subtire peste spdlog (consola + fisier).
// ============================================================================
class Logger {
public:
    static Logger& instance();

    // Initializare din configuratie (nivel: trace/debug/info/warn/error, fisier de log).
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

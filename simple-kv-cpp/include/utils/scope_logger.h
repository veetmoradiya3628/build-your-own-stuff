#pragma once

#include <iostream>
#include <string>

namespace kv
{

    /**
     * @brief A simple RAII-based scope logger for debugging.
     *
     * This class logs entry and exit of a scope to std::cout. It is useful for
     * tracing function calls or code blocks during development and debugging.
     */
    class ScopeLogger
    {
    public:
        /**
         * @brief Constructs a ScopeLogger with a scope name.
         *
         * Logs the entry message immediately.
         *
         * @param scopeName The name of the scope (e.g., function name).
         */
        explicit ScopeLogger(const std::string &scopeName)
            : scope(scopeName)
        {
            std::cout << "[ENTER] " << scope << std::endl;
        }

        /**
         * @brief Destructor that logs the exit message.
         */
        ~ScopeLogger()
        {
            std::cout << "[EXIT ] " << scope << std::endl;
        }

    private:
        /// The name of the scope being logged.
        std::string scope;
    };

} // namespace kv

#pragma once

#include <string>
#include <optional>

namespace kv
{

    /**
     * @brief Interface for a key-value store.
     *
     * This abstract class defines the basic operations that any key-value store
     * implementation must provide: storing a value under a key, retrieving a value
     * by key, and removing a key-value pair.
     */
    class IKeyValueStore
    {
    public:
        virtual ~IKeyValueStore() = default;

        /**
         * @brief Stores a value associated with a key.
         *
         * If the key already exists, its value is updated.
         *
         * @param key The key to store the value under. Must not be empty.
         * @param value The value to store.
         * @return true if the operation was successful, false otherwise (e.g., empty key).
         */
        virtual bool put(const std::string &key, const std::string &value) = 0;

        /**
         * @brief Retrieves the value associated with a key.
         *
         * @param key The key to look up.
         * @return An optional containing the value if the key exists, std::nullopt otherwise.
         */
        virtual std::optional<std::string> get(const std::string &key) const = 0;

        /**
         * @brief Removes the key-value pair associated with a key.
         *
         * @param key The key to remove.
         * @return true if the key was found and removed, false otherwise.
         */
        virtual bool remove(const std::string &key) = 0;
    };

}

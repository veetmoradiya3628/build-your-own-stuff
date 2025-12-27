#pragma once

#include <optional>
#include <string>

namespace kv
{

    /**
     * @brief Interface for a storage engine used by the key-value store.
     *
     * This abstract class defines the low-level storage operations that a storage
     * engine must implement. It provides a layer of abstraction over the actual
     * storage mechanism (e.g., in-memory, file-based, database).
     */
    class IStorageEngine
    {
    public:
        virtual ~IStorageEngine() = default;

        /**
         * @brief Writes or updates a key-value pair in the storage.
         *
         * @param key The key to write. Must not be empty.
         * @param value The value to associate with the key.
         * @return true if the write was successful, false otherwise.
         */
        virtual bool write(const std::string &key,
                           const std::string &value) = 0;

        /**
         * @brief Reads the value associated with a key from the storage.
         *
         * @param key The key to read.
         * @return An optional containing the value if the key exists, std::nullopt otherwise.
         */
        virtual std::optional<std::string>
        read(const std::string &key) const = 0;

        /**
         * @brief Removes a key-value pair from the storage.
         *
         * @param key The key to erase.
         * @return true if the key was found and erased, false otherwise.
         */
        virtual bool erase(const std::string &key) = 0;
    };

} // namespace kv

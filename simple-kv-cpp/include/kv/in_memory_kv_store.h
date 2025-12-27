#pragma once

#include "kv/storage_engine.h"
#include <unordered_map>

namespace kv
{

    /**
     * @brief In-memory implementation of the storage engine.
     *
     * This class provides a simple in-memory storage using std::unordered_map.
     * All data is stored in RAM and is lost when the program terminates.
     * It is suitable for testing, caching, or temporary storage needs.
     */
    class InMemoryStorageEngine : public IStorageEngine
    {
    public:
        /**
         * @brief Writes a key-value pair to the in-memory store.
         *
         * @param key The key to store. Must not be empty.
         * @param value The value to associate with the key.
         * @return true if successful, false if key is empty.
         */
        bool write(const std::string &key,
                   const std::string &value) override;

        /**
         * @brief Reads the value associated with a key.
         *
         * @param key The key to look up.
         * @return The value if found, std::nullopt otherwise.
         */
        std::optional<std::string>
        read(const std::string &key) const override;

        /**
         * @brief Erases a key-value pair from the store.
         *
         * @param key The key to remove.
         * @return true if the key was found and removed, false otherwise.
         */
        bool erase(const std::string &key) override;

    private:
        /// The underlying storage container.
        std::unordered_map<std::string, std::string> store_;
    };

} // namespace kv

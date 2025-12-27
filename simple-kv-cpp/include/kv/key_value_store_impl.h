#pragma once

#include "kv/key_value_store.h"
#include "kv/storage_engine.h"
#include <memory>

namespace kv
{

    /**
     * @brief Implementation of the key-value store using a storage engine.
     *
     * This class provides the high-level key-value store interface by delegating
     * operations to an underlying storage engine. It allows for different storage
     * backends (in-memory, file-based, etc.) to be plugged in.
     */
    class KeyValueStore : public IKeyValueStore
    {
    public:
        /**
         * @brief Constructs a KeyValueStore with the given storage engine.
         *
         * @param engine A unique pointer to the storage engine to use.
         *               Ownership is transferred to this object.
         */
        explicit KeyValueStore(std::unique_ptr<IStorageEngine> engine);

        /**
         * @brief Stores a value under a key.
         *
         * @param key The key to store under. Must not be empty.
         * @param value The value to store.
         * @return true if successful, false otherwise.
         */
        bool put(const std::string &key,
                 const std::string &value) override;

        /**
         * @brief Retrieves the value associated with a key.
         *
         * @param key The key to look up.
         * @return The value if found, std::nullopt otherwise.
         */
        std::optional<std::string>
        get(const std::string &key) const override;

        /**
         * @brief Removes a key-value pair.
         *
         * @param key The key to remove.
         * @return true if removed, false if not found.
         */
        bool remove(const std::string &key) override;

    private:
        /// The underlying storage engine.
        std::unique_ptr<IStorageEngine> engine_;
    };

} // namespace kv

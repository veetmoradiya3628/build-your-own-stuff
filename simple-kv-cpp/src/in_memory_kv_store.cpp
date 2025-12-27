#include "kv/in_memory_kv_store.h"
#include "utils/scope_logger.h"

namespace kv
{

    bool InMemoryStorageEngine::write(const std::string &key,
                                      const std::string &value)
    {
        ScopeLogger log("InMemoryStorageEngine::write");

        // Validate input: key must not be empty
        if (key.empty())
        {
            return false;
        }

        // Store or update the key-value pair
        store_[key] = value;
        return true;
    }

    std::optional<std::string>
    InMemoryStorageEngine::read(const std::string &key) const
    {
        ScopeLogger log("InMemoryStorageEngine::read");

        // Validate input: key must not be empty
        if (key.empty())
        {
            return std::nullopt;
        }

        // Look up the key in the map
        auto it = store_.find(key);
        if (it == store_.end())
        {
            return std::nullopt;
        }
        return it->second;
    }

    bool InMemoryStorageEngine::erase(const std::string &key)
    {
        ScopeLogger log("InMemoryStorageEngine::erase");

        // Validate input: key must not be empty
        if (key.empty())
        {
            return false;
        }

        // Attempt to erase the key
        return store_.erase(key) > 0;
    }

} // namespace kv

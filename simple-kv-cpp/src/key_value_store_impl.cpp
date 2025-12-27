#include "kv/key_value_store_impl.h"
#include <stdexcept>

namespace kv
{

    KeyValueStore::KeyValueStore(std::unique_ptr<IStorageEngine> engine)
        : engine_(std::move(engine))
    {
        // Ensure the engine is valid
        if (!engine_)
        {
            throw std::invalid_argument("Storage engine cannot be null");
        }
    }

    bool KeyValueStore::put(const std::string &key,
                            const std::string &value)
    {
        // Delegate to the storage engine
        return engine_->write(key, value);
    }

    std::optional<std::string>
    KeyValueStore::get(const std::string &key) const
    {
        // Delegate to the storage engine
        return engine_->read(key);
    }

    bool KeyValueStore::remove(const std::string &key)
    {
        // Delegate to the storage engine
        return engine_->erase(key);
    }

} // namespace kv

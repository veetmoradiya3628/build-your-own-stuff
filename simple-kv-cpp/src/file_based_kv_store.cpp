#include "kv/file_based_kv_store.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace kv
{

    FileBasedStorageEngine::FileBasedStorageEngine(const std::filesystem::path& filePath)
        : file_path_(filePath)
    {
        loadFromFile();
    }

    bool FileBasedStorageEngine::write(const std::string &key,
                                       const std::string &value)
    {
        // Validate input: key must not be empty
        if (key.empty())
        {
            return false;
        }

        // Update in-memory store
        store_[key] = value;

        // Persist to file
        saveToFile();
        return true;
    }

    std::optional<std::string>
    FileBasedStorageEngine::read(const std::string &key) const
    {
        // Validate input: key must not be empty
        if (key.empty())
        {
            return std::nullopt;
        }

        // Look up in in-memory store
        auto it = store_.find(key);
        if (it == store_.end())
        {
            return std::nullopt;
        }
        return it->second;
    }

    bool FileBasedStorageEngine::erase(const std::string &key)
    {
        // Validate input: key must not be empty
        if (key.empty())
        {
            return false;
        }

        // Erase from in-memory store
        auto erased = store_.erase(key) > 0;

        // Persist to file if something was erased
        if (erased)
        {
            saveToFile();
        }
        return erased;
    }

    void FileBasedStorageEngine::loadFromFile()
    {
        store_.clear();
        std::ifstream file(file_path_);
        if (!file.is_open())
        {
            // File doesn't exist, start with empty store
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            // Parse key=value
            size_t pos = line.find('=');
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                store_[key] = value;
            }
        }
    }

    void FileBasedStorageEngine::saveToFile() const
    {
        std::ofstream file(file_path_);
        if (!file.is_open())
        {
            std::cerr << "Error: Cannot open file for writing: " << file_path_ << std::endl;
            return;
        }

        for (const auto& [key, value] : store_)
        {
            file << key << "=" << value << "\n";
        }
    }

} // namespace kv
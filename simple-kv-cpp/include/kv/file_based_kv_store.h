#pragma once

#include "kv/storage_engine.h"
#include <unordered_map>
#include <string>
#include <filesystem>

namespace kv
{

    /**
     * @brief File-based implementation of the storage engine.
     *
     * This class provides persistent storage using a text file on disk.
     * Data is stored as key=value pairs, one per line.
     * All data is loaded into memory on construction and saved back to file
     * on every write or erase operation for simplicity and durability.
     */
    class FileBasedStorageEngine : public IStorageEngine
    {
    public:
        /**
         * @brief Constructs a FileBasedStorageEngine with the specified file path.
         *
         * Loads existing data from the file if it exists.
         *
         * @param filePath The path to the storage file.
         */
        explicit FileBasedStorageEngine(const std::filesystem::path& filePath);

        /**
         * @brief Writes a key-value pair to the storage and persists to file.
         *
         * @param key The key to store. Must not be empty.
         * @param value The value to associate with the key.
         * @return true if successful, false if key is empty.
         */
        bool write(const std::string &key,
                   const std::string &value) override;

        /**
         * @brief Reads the value associated with a key from storage.
         *
         * @param key The key to look up.
         * @return The value if found, std::nullopt otherwise.
         */
        std::optional<std::string>
        read(const std::string &key) const override;

        /**
         * @brief Erases a key-value pair from storage and persists to file.
         *
         * @param key The key to remove.
         * @return true if the key was found and removed, false otherwise.
         */
        bool erase(const std::string &key) override;

    private:
        /**
         * @brief Loads data from the file into the in-memory store.
         */
        void loadFromFile();

        /**
         * @brief Saves the entire in-memory store to the file.
         */
        void saveToFile() const;

        /// The file path for persistence.
        std::filesystem::path file_path_;

        /// The in-memory storage container.
        std::unordered_map<std::string, std::string> store_;
    };

} // namespace kv
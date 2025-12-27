#include "kv/key_value_store_impl.h"
#include "kv/in_memory_kv_store.h"
#include "kv/file_based_kv_store.h"
#include <iostream>
#include <string>
#include <memory>
#include <filesystem>

using namespace kv;

/**
 * @brief Simple command-line utility for testing the key-value store.
 *
 * Supports both in-memory and file-based storage engines.
 * Commands:
 * - put <key> <value> : Store a value
 * - get <key> : Retrieve a value
 * - remove <key> : Remove a key
 * - list : List all keys
 * - exit : Quit
 */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <storage_type> [file_path]\n";
        std::cout << "Storage types: memory, file\n";
        std::cout << "For file storage, provide the file path as second argument.\n";
        return 1;
    }

    std::string storage_type = argv[1];
    std::unique_ptr<IStorageEngine> engine;

    if (storage_type == "memory")
    {
        engine = std::make_unique<InMemoryStorageEngine>();
        std::cout << "Using in-memory storage.\n";
    }
    else if (storage_type == "file")
    {
        if (argc < 3)
        {
            std::cout << "File path required for file storage.\n";
            return 1;
        }
        std::filesystem::path file_path = argv[2];
        engine = std::make_unique<FileBasedStorageEngine>(file_path);
        std::cout << "Using file-based storage: " << file_path << "\n";
    }
    else
    {
        std::cout << "Invalid storage type. Use 'memory' or 'file'.\n";
        return 1;
    }

    KeyValueStore store(std::move(engine));

    std::cout << "Key-Value Store Developer Test Utility\n";
    std::cout << "Commands: put <key> <value>, get <key>, remove <key>, list, exit\n";

    std::string command;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command.empty())
            continue;

        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        if (cmd == "exit")
        {
            break;
        }
        else if (cmd == "put")
        {
            std::string key, value;
            iss >> key;
            std::getline(iss, value);
            // Remove leading space
            if (!value.empty() && value[0] == ' ')
                value = value.substr(1);

            if (store.put(key, value))
            {
                std::cout << "Stored: " << key << " = " << value << "\n";
            }
            else
            {
                std::cout << "Failed to store (empty key?)\n";
            }
        }
        else if (cmd == "get")
        {
            std::string key;
            iss >> key;
            auto result = store.get(key);
            if (result.has_value())
            {
                std::cout << key << " = " << result.value() << "\n";
            }
            else
            {
                std::cout << "Key not found: " << key << "\n";
            }
        }
        else if (cmd == "remove")
        {
            std::string key;
            iss >> key;
            if (store.remove(key))
            {
                std::cout << "Removed: " << key << "\n";
            }
            else
            {
                std::cout << "Key not found: " << key << "\n";
            }
        }
        else if (cmd == "list")
        {
            // Note: This is a limitation - we can't list keys easily with current API
            // In a real implementation, we'd add a list method to the interface
            std::cout << "List command not implemented in current API.\n";
            std::cout << "To list keys, you'd need to extend the interface.\n";
        }
        else
        {
            std::cout << "Unknown command. Try: put, get, remove, list, exit\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
# Simple Key-Value Store in C++

An educational implementation of a key-value store in C++20, featuring an in-memory storage engine and a clean interface design.

## Features

- **Interface-based design**: Clean separation between the key-value store API and storage engines
- **Multiple storage engines**: In-memory for speed, file-based for persistence
- **Extensible**: Easy to add new storage engines (e.g., database-backed)
- **Comprehensive tests**: Extensive unit tests covering edge cases and error conditions
- **Modern C++**: Uses C++20 features and best practices
- **Developer utility**: Command-line tool for interactive testing

## Project Structure

```
├── CMakeLists.txt          # Main CMake configuration
├── src/                    # Source files
│   ├── CMakeLists.txt
│   ├── in_memory_kv_store.cpp
│   ├── key_value_store_impl.cpp
│   └── file_based_kv_store.cpp
├── include/                # Header files
│   ├── kv/
│   │   ├── key_value_store.h
│   │   ├── storage_engine.h
│   │   ├── in_memory_kv_store.h
│   │   ├── key_value_store_impl.h
│   │   └── file_based_kv_store.h
│   └── utils/
│       └── scope_logger.h
└── tests/                  # Unit tests and utilities
    ├── CMakeLists.txt
    ├── kvstore_test.cpp
    └── developer_test.cpp
```

## Building

### Prerequisites

- CMake 3.16 or later
- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- Google Test (gtest) library

### Build Steps

```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests

```bash
make test
# or
./tests/kvstore_tests
```

### Developer Test Utility

A command-line utility for interactive testing of the key-value store:

```bash
# Test with in-memory storage
./tests/developer_test memory

# Test with file-based storage
./tests/developer_test file my_store.txt
```

Commands in the utility:
- `put <key> <value>` - Store a value
- `get <key>` - Retrieve a value  
- `remove <key>` - Remove a key
- `list` - List all keys (not implemented in current API)
- `exit` - Quit

## Usage Example

### In-Memory Storage

```cpp
#include "kv/key_value_store_impl.h"
#include "kv/in_memory_kv_store.h"
#include <memory>

int main() {
    // Create an in-memory storage engine
    auto engine = std::make_unique<kv::InMemoryStorageEngine>();
    kv::KeyValueStore store(std::move(engine));

    store.put("name", "Alice");
    auto name = store.get("name");
    // ...
}
```

### File-Based Persistent Storage

```cpp
#include "kv/key_value_store_impl.h"
#include "kv/file_based_kv_store.h"
#include <memory>
#include <filesystem>

int main() {
    // Create a file-based storage engine
    std::filesystem::path file_path = "my_store.txt";
    auto engine = std::make_unique<kv::FileBasedStorageEngine>(file_path);
    kv::KeyValueStore store(std::move(engine));

    store.put("name", "Alice");
    store.put("age", "30");

    // Data persists across program runs
    return 0;
}
```

## API Reference

### IKeyValueStore

- `bool put(const std::string& key, const std::string& value)`: Stores a value under a key
- `std::optional<std::string> get(const std::string& key) const`: Retrieves a value by key
- `bool remove(const std::string& key)`: Removes a key-value pair

### IStorageEngine

- `bool write(const std::string& key, const std::string& value)`: Low-level write operation
- `std::optional<std::string> read(const std::string& key) const`: Low-level read operation
- `bool erase(const std::string& key)`: Low-level erase operation

## Design Decisions

- **Interface Segregation**: Separate interfaces for high-level KV operations and low-level storage
- **RAII**: Proper resource management with smart pointers
- **Error Handling**: Consistent validation (e.g., empty keys are rejected)
- **Extensibility**: Easy to swap storage engines without changing client code
- **Testing**: Comprehensive test coverage including edge cases

## Future Enhancements

- Database-backed storage engine (SQL/NoSQL)
- Persistent storage with advanced serialization (JSON, binary)
- Concurrent access with thread safety
- LRU cache with size limits
- Network interface for distributed storage
- Key listing and iteration API
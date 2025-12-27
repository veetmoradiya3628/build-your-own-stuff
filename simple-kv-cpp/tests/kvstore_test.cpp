#include <gtest/gtest.h>
#include "kv/key_value_store_impl.h"
#include "kv/in_memory_kv_store.h"
#include "kv/file_based_kv_store.h"
#include <memory>
#include <filesystem>
#include <fstream>

using namespace kv;

// Test fixture for KeyValueStore tests
class KeyValueStoreTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        engine_ = std::make_unique<InMemoryStorageEngine>();
        store_ = std::make_unique<KeyValueStore>(std::move(engine_));
    }

    std::unique_ptr<IStorageEngine> engine_;
    std::unique_ptr<KeyValueStore> store_;
};

// Basic put and get
TEST_F(KeyValueStoreTest, BasicPutGet)
{
    EXPECT_TRUE(store_->put("name", "veet"));
    auto value = store_->get("name");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), "veet");
}

// Test updating a value
TEST_F(KeyValueStoreTest, UpdateValue)
{
    EXPECT_TRUE(store_->put("key1", "value1"));
    EXPECT_TRUE(store_->put("key1", "updated_value"));
    auto value = store_->get("key1");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), "updated_value");
}

// Test getting non-existent key
TEST_F(KeyValueStoreTest, GetNonExistentKey)
{
    auto value = store_->get("nonexistent");
    EXPECT_FALSE(value.has_value());
}

// Test removing a key
TEST_F(KeyValueStoreTest, RemoveKey)
{
    EXPECT_TRUE(store_->put("key1", "value1"));
    EXPECT_TRUE(store_->remove("key1"));
    auto value = store_->get("key1");
    EXPECT_FALSE(value.has_value());
}

// Test removing non-existent key
TEST_F(KeyValueStoreTest, RemoveNonExistentKey)
{
    EXPECT_FALSE(store_->remove("nonexistent"));
}

// Test empty key handling
TEST_F(KeyValueStoreTest, EmptyKeyPut)
{
    EXPECT_FALSE(store_->put("", "value"));
}

TEST_F(KeyValueStoreTest, EmptyKeyGet)
{
    auto value = store_->get("");
    EXPECT_FALSE(value.has_value());
}

TEST_F(KeyValueStoreTest, EmptyKeyRemove)
{
    EXPECT_FALSE(store_->remove(""));
}

// Test multiple keys
TEST_F(KeyValueStoreTest, MultipleKeys)
{
    EXPECT_TRUE(store_->put("key1", "value1"));
    EXPECT_TRUE(store_->put("key2", "value2"));
    EXPECT_TRUE(store_->put("key3", "value3"));

    auto v1 = store_->get("key1");
    auto v2 = store_->get("key2");
    auto v3 = store_->get("key3");

    ASSERT_TRUE(v1.has_value());
    ASSERT_TRUE(v2.has_value());
    ASSERT_TRUE(v3.has_value());

    EXPECT_EQ(v1.value(), "value1");
    EXPECT_EQ(v2.value(), "value2");
    EXPECT_EQ(v3.value(), "value3");
}

// Test empty value
TEST_F(KeyValueStoreTest, EmptyValue)
{
    EXPECT_TRUE(store_->put("key", ""));
    auto value = store_->get("key");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), "");
}

// Test large key and value
TEST_F(KeyValueStoreTest, LargeKeyValue)
{
    std::string large_key(1000, 'a');
    std::string large_value(10000, 'b');

    EXPECT_TRUE(store_->put(large_key, large_value));
    auto value = store_->get(large_key);
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), large_value);
}

// Test special characters
TEST_F(KeyValueStoreTest, SpecialCharacters)
{
    EXPECT_TRUE(store_->put("key@#$%", "value!@#$"));
    auto value = store_->get("key@#$%");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), "value!@#$");
}

// Test remove and re-add
TEST_F(KeyValueStoreTest, RemoveAndReAdd)
{
    EXPECT_TRUE(store_->put("key", "value1"));
    EXPECT_TRUE(store_->remove("key"));
    EXPECT_FALSE(store_->get("key").has_value());
    EXPECT_TRUE(store_->put("key", "value2"));
    auto value = store_->get("key");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), "value2");
}

// Test constructor with null engine
TEST(KeyValueStoreConstructorTest, NullEngine)
{
    EXPECT_THROW(KeyValueStore(nullptr), std::invalid_argument);
}

// Test InMemoryStorageEngine directly
class InMemoryStorageEngineTest : public ::testing::Test
{
protected:
    InMemoryStorageEngine engine_;
};

TEST_F(InMemoryStorageEngineTest, WriteRead)
{
    EXPECT_TRUE(engine_.write("key", "value"));
    auto value = engine_.read("key");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), "value");
}

TEST_F(InMemoryStorageEngineTest, WriteEmptyKey)
{
    EXPECT_FALSE(engine_.write("", "value"));
}

TEST_F(InMemoryStorageEngineTest, ReadEmptyKey)
{
    auto value = engine_.read("");
    EXPECT_FALSE(value.has_value());
}

TEST_F(InMemoryStorageEngineTest, EraseEmptyKey)
{
    EXPECT_FALSE(engine_.erase(""));
}

TEST_F(InMemoryStorageEngineTest, EraseNonExistent)
{
    EXPECT_FALSE(engine_.erase("nonexistent"));
}

TEST_F(InMemoryStorageEngineTest, Overwrite)
{
    EXPECT_TRUE(engine_.write("key", "value1"));
    EXPECT_TRUE(engine_.write("key", "value2"));
    auto value = engine_.read("key");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), "value2");
}

// Test fixture for FileBasedStorageEngine tests
class FileBasedStorageEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Use a temporary file for testing
        temp_file_ = std::filesystem::temp_directory_path() / "test_kv_store.txt";
        // Clean up any existing file
        std::filesystem::remove(temp_file_);
        engine_ = std::make_unique<FileBasedStorageEngine>(temp_file_);
    }

    void TearDown() override {
        // Clean up the test file
        std::filesystem::remove(temp_file_);
    }

    std::filesystem::path temp_file_;
    std::unique_ptr<FileBasedStorageEngine> engine_;
};

TEST_F(FileBasedStorageEngineTest, WriteRead) {
    EXPECT_TRUE(engine_->write("key", "value"));
    auto value = engine_->read("key");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), "value");
}

TEST_F(FileBasedStorageEngineTest, Persistence) {
    EXPECT_TRUE(engine_->write("persistent", "data"));
    // Create a new engine instance to test loading from file
    auto new_engine = std::make_unique<FileBasedStorageEngine>(temp_file_);
    auto value = new_engine->read("persistent");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), "data");
}

TEST_F(FileBasedStorageEngineTest, Erase) {
    EXPECT_TRUE(engine_->write("key", "value"));
    EXPECT_TRUE(engine_->erase("key"));
    auto value = engine_->read("key");
    EXPECT_FALSE(value.has_value());
}

TEST_F(FileBasedStorageEngineTest, EmptyKey) {
    EXPECT_FALSE(engine_->write("", "value"));
    EXPECT_FALSE(engine_->read("").has_value());
    EXPECT_FALSE(engine_->erase(""));
}

TEST_F(FileBasedStorageEngineTest, MultipleKeys) {
    EXPECT_TRUE(engine_->write("key1", "value1"));
    EXPECT_TRUE(engine_->write("key2", "value2"));
    EXPECT_TRUE(engine_->write("key3", "value3"));

    auto v1 = engine_->read("key1");
    auto v2 = engine_->read("key2");
    auto v3 = engine_->read("key3");

    ASSERT_TRUE(v1.has_value());
    ASSERT_TRUE(v2.has_value());
    ASSERT_TRUE(v3.has_value());

    EXPECT_EQ(v1.value(), "value1");
    EXPECT_EQ(v2.value(), "value2");
    EXPECT_EQ(v3.value(), "value3");
}

// Test KeyValueStore with FileBasedStorageEngine
TEST(FileBasedKeyValueStoreTest, BasicOperations) {
    auto temp_file = std::filesystem::temp_directory_path() / "test_kv_full.txt";
    std::filesystem::remove(temp_file); // Clean up

    auto engine = std::make_unique<FileBasedStorageEngine>(temp_file);
    KeyValueStore store(std::move(engine));

    EXPECT_TRUE(store.put("name", "Alice"));
    EXPECT_TRUE(store.put("age", "30"));

    auto name = store.get("name");
    ASSERT_TRUE(name.has_value());
    EXPECT_EQ(name.value(), "Alice");

    EXPECT_TRUE(store.remove("age"));
    auto age = store.get("age");
    EXPECT_FALSE(age.has_value());

    // Test persistence by creating new store
    auto new_engine = std::make_unique<FileBasedStorageEngine>(temp_file);
    KeyValueStore new_store(std::move(new_engine));
    auto persisted_name = new_store.get("name");
    ASSERT_TRUE(persisted_name.has_value());
    EXPECT_EQ(persisted_name.value(), "Alice");

    std::filesystem::remove(temp_file); // Clean up
}

package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"os"
	"sync"
)

type LogEntry struct {
	Type  string `json:"type"`
	Key   string `json:"key"`
	Value string `json:"value"`
}

type KVStore struct {
	store   map[string]string
	logFile *os.File
	mu      sync.Mutex
}

func NewKVStore() (*KVStore, error) {
	file, err := os.OpenFile("wal.log", os.O_CREATE|os.O_RDWR|os.O_APPEND, 0666)
	if err != nil {
		return nil, err
	}
	kv := &KVStore{
		store:   make(map[string]string),
		logFile: file,
	}
	if err := kv.recoverFromWAL(); err != nil {
		return nil, err
	}

	return kv, nil
}

func (kv *KVStore) recoverFromWAL() error {
	_, err := kv.logFile.Seek(0, 0)
	if err != nil {
		return err
	}

	scanner := bufio.NewScanner(kv.logFile)
	for scanner.Scan() {
		line := scanner.Bytes()
		var entry LogEntry
		if err := json.Unmarshal(line, &entry); err != nil {
			continue
		}
		switch entry.Type {
		case "put":
			kv.store[entry.Key] = entry.Value
		case "delete":
			delete(kv.store, entry.Key)
		}
	}
	return scanner.Err()
}

// Put adds or updates a key-value pair
func (kv *KVStore) Put(key, value string) error {
	entry := LogEntry{Type: "put", Key: key, Value: value}
	if err := kv.writeToWAL(entry); err != nil {
		return err
	}

	kv.mu.Lock()
	defer kv.mu.Unlock()
	kv.store[key] = value
	return nil
}

// Delete removes a key from the store
func (kv *KVStore) Delete(key string) error {
	entry := LogEntry{Type: "delete", Key: key}
	if err := kv.writeToWAL(entry); err != nil {
		return err
	}

	kv.mu.Lock()
	defer kv.mu.Unlock()
	delete(kv.store, key)
	return nil
}

// writeToWAL logs the operation before applying
func (kv *KVStore) writeToWAL(entry LogEntry) error {
	data, err := json.Marshal(entry)
	if err != nil {
		return err
	}

	_, err = kv.logFile.Write(append(data, '\n'))
	return err
}

// Display prints the current store state
func (kv *KVStore) Display() {
	fmt.Println("Current Key-Value Store:")
	for k, v := range kv.store {
		fmt.Printf("%s: %s\n", k, v)
	}
}

func (kv *KVStore) Close() {
	kv.logFile.Close()
}

func main() {
	fmt.Println("Welcome to the WAL KV Store!")
	kv, err := NewKVStore()
	if err != nil {
		fmt.Println("Error initializing KV Store:", err)
		return
	}
	defer kv.Close()

	fmt.Println("\n--- Performing operations ---")
	kv.Put("name", "Alice")
	kv.Put("lang", "Go")
	kv.Delete("lang")

	// Display store
	kv.Display()

	// Simulate restart
	fmt.Println("\n--- Simulating Restart ---")
	newKV, err := NewKVStore()
	if err != nil {
		panic(err)
	}
	defer newKV.Close()

	newKV.Display()
}

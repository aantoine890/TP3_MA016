#ifndef HASH_TABLES_H
#define HASH_TABLES_H

// Define the structure for a generic hash table
typedef struct HashTable HashTable;

// Function to create a new hash table
HashTable* createHashTable(int size);

// Function to insert a key-value pair into the hash table
void insert(HashTable* table, const char* key, void* value);

// Function to retrieve a value based on a key
void* get(HashTable* table, const char* key);

// Function to remove a key-value pair from the hash table
void removeKey(HashTable* table, const char* key);

// Function to destroy the hash table and free memory
void destroyHashTable(HashTable* table);

#endif

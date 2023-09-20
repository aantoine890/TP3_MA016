#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a key-value pair
typedef struct{
    char* key;
    void* value;
    struct KeyValuePair* next;
} KeyValuePair;

// Define the structure for the hash table
typedef struct{
    int size;
    KeyValuePair** table;
}  HashTable ;

// Function to create a new hash table
HashTable* createHashTable(int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->table = (KeyValuePair**)malloc(sizeof(KeyValuePair*) * size);
    memset(table->table, 0, sizeof(KeyValuePair*) * size);
    return table;
}

// Function to calculate the hash value for a key
int hash(const char* key, int size) {
    int hash = 0;
    for (int i = 0; key[i]; i++) {
        hash += key[i];
    }
    return hash % size;
}

// Function to insert a key-value pair into the hash table
void insert(HashTable* table, const char* key, void* value) {
    int index = hash(key, table->size);
    KeyValuePair* newPair = (KeyValuePair*)malloc(sizeof(KeyValuePair));
    newPair->key = strdup(key);
    newPair->value = value;
    newPair->next = table->table[index];
    table->table[index] = newPair;
}

// Function to retrieve a value based on a key
void* get(HashTable* table, const char* key) {
    int index = hash(key, table->size);
    KeyValuePair* current = table->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

// Function to remove a key-value pair from the hash table
void removeKey(HashTable* table, const char* key) {
    int index = hash(key, table->size);
    KeyValuePair* current = table->table[index];
    KeyValuePair* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                table->table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->key);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Function to destroy the hash table and free memory
void destroyHashTable(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        KeyValuePair* current = table->table[i];
        while (current != NULL) {
            KeyValuePair* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table->table);
    free(table);
}

// Simple hash table implemented in C.

#include "ht.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Hash table entry (slot may be filled or empty).
typedef struct {
    const char* key;  // key is NULL if this slot is empty
    void* value;
} ht_entry;

// Hash table structure: create with ht_create, free with ht_destroy.
struct ht {
    ht_entry* entries;  // hash slots
    size_t capacity;    // size of _entries array
    size_t length;      // number of items in hash table
};

#define INITIAL_CAPACITY 16  // must not be zero

ht* ht_create(void) {
    ht* table = malloc(sizeof(ht));
    if (table == NULL)
    {
        return NULL;
    }
    table->length = 0;
    table->capacity = INITIAL_CAPACITY;
    table->entries = calloc(table->capacity, sizeof(ht_entry));
    if (table->entries == NULL)
    {
        free(table);
        return NULL;
    }
    return table;
}

void ht_destroy(ht* table) {
    for (size_t i = 0; i < table->capacity; i++)
    {
        free((void*)table->entries[i].key);
    }
    free(table->entries);
    free(table);
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

void* ht_get(ht* table, const char* key) {
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

    while (table->entries[index].key != NULL)
    {
        if (strcmp(table->entries[index].key, key) == 0)
        {
            return table->entries[index].value;
        }
        index++;
        if (index >= table->capacity)
        {
            index = 0;
        }
    }
    return NULL;
}

// Internal function to set an entry (without expanding table).
static const char* ht_set_entry(ht_entry* entries, size_t capacity,
                                const char* key, void* value, size_t* plength) 
{
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    while (entries[index].key != NULL)
    {
        if (strcmp(entries[index].key, key) == 0)
        {
            // 更新现有键值对
            entries[index].value = value;
            return entries[index].key;
        }
        index++;
        if (index >= capacity)
        {
            index = 0;
        }
    }
    if (plength != NULL)
    {
        key = strdup(key);
        if (key == NULL)
        {
            return NULL;
        }
        *plength += 1;
    }
    entries[index].key = key;
    entries[index].value = value;
    return key;
}

// Expand hash table to twice its current size. Return true on success,
// false if out of memory.
static bool ht_expand(ht* table) {
    size_t new_capacity = table->capacity * 2;
    assert(new_capacity > table->capacity); // 避免溢出
    if (new_capacity < table->capacity)
    {
        return false;
    }
    ht_entry* new_entries = calloc(new_capacity, sizeof(ht_entry));
    assert(new_entries != NULL); // 确保分配内存成功
    if (new_entries == NULL)
    {
        return false;
    }

    for (size_t i = 0; i < table->capacity; i++)
    {
        ht_entry entry = table->entries[i];
        if (entry.key != NULL)
        {
            ht_set_entry(new_entries, new_capacity, entry.key,
                         entry.value, NULL);
        }
    }
    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

const char* ht_set(ht* table, const char* key, void* value) {
    if (value == NULL)
    {
        return NULL;
    }

    if (table->length >= table->capacity / 2)
    {
        if (!ht_expand(table))
        {
            return NULL;
        }
    }
    return ht_set_entry(table->entries, table->capacity, key, value, &table->length);
}

size_t ht_length(ht* table) {
    return table->length;
}

hti ht_iterator(ht* table) {
    hti it;
    it._table = table;
    it._index = 0;
    return it;
}

bool ht_next(hti* it) {
    ht* table = it->_table;
    while (it->_index < table->capacity)
    {
        size_t i = it->_index++;
        if (table->entries[i].key != NULL)
        {
            it->key = table->entries[i].key;
            it->value = table->entries[i].value;
            return true;
        }
    }
    return false;
}

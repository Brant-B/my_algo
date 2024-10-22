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
    //todo
}

void ht_destroy(ht* table) {
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
}

// Internal function to set an entry (without expanding table).
static const char* ht_set_entry(ht_entry* entries, size_t capacity,
        const char* key, void* value, size_t* plength) {
}

// Expand hash table to twice its current size. Return true on success,
// false if out of memory.
static bool ht_expand(ht* table) {
}

const char* ht_set(ht* table, const char* key, void* value) {
}

size_t ht_length(ht* table) {
}

hti ht_iterator(ht* table) {
}

bool ht_next(hti* it) {
}

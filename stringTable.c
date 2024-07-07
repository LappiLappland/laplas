
#include <string.h>

#ifdef DEBUG_STRINGS
#include <stdio.h>
#endif

#include "object.h"
#include "stringTable.h"
#include "memory.h"


LString* graveyard;

inline static bool isTombstone(LString* str) {
  return str->length == 0;
}

void LSTInit(StringTable* table) {
  table->capacity = 0;
  table->count = 0;
  table->entries = NULL;
}

void LSTFree(StringTable* table) {
  FREE_ARRAY(LString, table->entries, table->capacity);
  LSTInit(table);
}

uint32_t hashString(const char* chars, size_t length) {
  uint32_t hash = 2166136261u;
  for (int i = 0; i < length; i++) {
    hash ^= (uint8_t)chars[i];
    hash *= 16777619;
  }
  return hash;
}

static LString* findSlot(LString* entries, size_t capacity, LString* str) {
  uint32_t index = str->hash & (capacity - 1);
  LString* tombstone = NULL;

  for (;;) {
    LString* entry = &entries[index];
    if (entry == graveyard) {
      if (tombstone == NULL) tombstone = entry;
    } else if (isTombstone(entry)) {
      return tombstone != NULL ? tombstone : entry;
    } else if (entry == str) {
      return entry;
    }

    index = (index + 1) & (capacity - 1);
  }
}

LString* LSTFind(StringTable* table, uint32_t hash, const char* str, size_t length) {
  if (table->count == 0) return NULL;

  uint32_t index = hash & (table->capacity - 1);
  for (;;) {
    LString* entry = &table->entries[index];
    if (isTombstone(entry)) {
      return NULL;
    } else if (entry->length == length &&
               entry->hash == hash &&
               memcmp(entry->content, str, length) == 0) {
      return entry;
    }

    index = (index + 1) & (table->capacity - 1);
  }
}

static void rehashStringTable(StringTable* table, size_t newCapacity) {
  LString* entries = ALLOCATE(LString, newCapacity);
  //for (size_t i = 0; i < newCapacity; i++)

  table->count = 0;
  for (size_t i = 0; i < table->capacity; i++) {
    LString* entry = &table->entries[i];
    if (entry == NULL) continue;

    LString* slot = findSlot(entries, newCapacity, entry);
    *slot = *entry;
    table->count++;
  }

  FREE_ARRAY(LString, table->entries, table->capacity);
  table->entries = entries;
  table->capacity = newCapacity;
}

LString* LSTSet(StringTable* table, LString* str) {
#ifdef DEBUG_STRINGS
  printf("inserting %.*s (%d / %f)\n", str->length, str->content, table->count,table->capacity * STRINGTABLE_MAX_LOAD);
#endif
  if (table->count + 1 > table->capacity * STRINGTABLE_MAX_LOAD) {
    size_t newCapacity = table->capacity < STRINGTABLE_INITIAL_SIZE ? STRINGTABLE_INITIAL_SIZE : table->capacity * 2;
    rehashStringTable(table, newCapacity);
  }

  LString* entry = findSlot(table->entries, table->capacity, str);
  table->count++;
  *entry = *str;
  return entry;
}

bool LSTtringDelete(StringTable* table, LString* str) {
  if (table->count == 0) return false;

  LString* entry = findSlot(table->entries, table->capacity, str);
  if (entry == NULL) return false;

  //TODO: since string remains, if checking hash equals tombstone hash then return this string length, so it's kind of like a cache for recently removed strings
  entry->length = 0;
  return true;
}
#ifndef laplas_stringTable_h
#define laplas_stringTable_h

#include "common.h"
#include "value.h"

#define STRINGTABLE_INITIAL_SIZE 8
#define STRINGTABLE_MAX_LOAD 0.75

typedef struct stringTable {
  LString* entries;
  int count;
  int capacity;
} StringTable;


void LSTInit(StringTable* table);
void LSTFree(StringTable* table);
LString* LSTFind(StringTable* table, uint32_t hash, const char* str, size_t length);
uint32_t hashString(const char* chars, size_t length);
LString* LSTSet(StringTable* table, LString* str);
bool LSTtringDelete(StringTable* table, LString* str);

#endif
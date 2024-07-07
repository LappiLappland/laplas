#ifndef laplas_object_h
#define laplas_object_h

#include "common.h"
#include "globalState.h"

typedef enum {
  LTString,
} LType;

typedef struct lObj {
  LType type;
  struct lObj* next; 
} LObj;

typedef struct lString {
  LObj obj;
  char* content;
  size_t length;
  uint32_t hash;
  uint8_t reservedId;
} LString;

LString* newLString(GlobalState* GS, char* chars, size_t length);

#endif
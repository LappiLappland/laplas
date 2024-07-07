
#ifdef DEBUG_STRINGS
#include <stdio.h>
#endif
#include <string.h>

#include "object.h"
#include "memory.h"
#include "stringTable.h"
#include "globalState.h"

#define ALLOCATE_LOBJ(type, LType) \
    allocateObject(sizeof(type), LType)

static LObj* allocateObject(size_t size, LType type) {
  LObj* object = reallocate(NULL, 0, size);
  object->type = type;
  object->next; //TODO: set next

  return object;
}

static LString* allocateString(GlobalState* GS, char* chars, size_t length, uint32_t hash) {
  LString* string = (LString*)ALLOCATE_LOBJ(LString, LTString);
  string->length = length;
  string->hash = hash;
  string->reservedId = 0;

  char* cloned = REALLOCATE_ARRAY(char, NULL, 0, length);
  memcpy(cloned, chars, length * sizeof(char));
  string->content = cloned;

  return LSTSet(&GS->LST, string);
}

LString* newLString(GlobalState* GS, char* chars, size_t length) {
  uint32_t hash = hashString(chars, length);
  GS->LST;
  LString* intern = LSTFind(&GS->LST, hash, chars, length);

  if (intern != NULL) {
    return intern;
  }


  return allocateString(GS, chars, length, hash);
}
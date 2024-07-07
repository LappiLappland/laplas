#ifndef laplas_globalState_h
#define laplas_globalState_h

#include "common.h"
#include "stringTable.h"

typedef struct {
  StringTable LST;
} GlobalState;

void initGlobalState(GlobalState* GS);

#endif
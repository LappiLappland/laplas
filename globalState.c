
#include "globalState.h"

void initGlobalState(GlobalState* GS) {
  StringTable LST;
  LSTInit(&LST);
  GS->LST = LST;
}
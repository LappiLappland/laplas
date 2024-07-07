#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "fileReader.h"
#include "globalState.h"
#include "object.h"

//TODO: remove later
#include "scanner.h"

static void processFile(const char* path) {
  char* text;
  switch (readFile(path, &text))
  {
    case FILE_READ_CANT_OPEN:
      fprintf(stderr, "Could not open file \"%s\"\n", path);
      exit(74);
    case FILE_READ_OUT_OF_MEMORY:
      fprintf(stderr, "Out of memory to store file \"%s\"\n", path);
      exit(74);
    case FILE_READ_SUCCESS:
  }

  printf("%s\n", text);
  
  GlobalState GS;
  initGlobalState(&GS);

  Scanner scanner;
  initScanner(&scanner, &GS, text);
  scanToken(&scanner, &GS);
  scanToken(&scanner, &GS);
  scanToken(&scanner, &GS);
  Token token = scanToken(&scanner, &GS);
  printf("lol -> %d\n", token.type);
}

int main(int argc, const char* argv[]) {

  //TODO: later remove this debug stuff
  if (argc == 2 || true) {
    processFile("../test/simple");
  } else {
    fprintf(stderr, "Usage: laplas path\n");
    exit(64);
  }

  return 0; 
}
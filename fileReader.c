#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "fileReader.h"

FileReaderResult readFile(const char* path, char** buffer) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    return FILE_READ_CANT_OPEN;
  }

  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char* text = malloc(fileSize + 1);
  if (text == NULL) {
    return FILE_READ_OUT_OF_MEMORY;
  }

  size_t fileBytes = fread(text, sizeof(char), fileSize, file);

  text[fileBytes] = '\0';

  *buffer = text;
  fclose(file);
  return FILE_READ_SUCCESS;
}
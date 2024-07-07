#ifndef laplas_fileReader_h
#define laplas_fileReader_h

typedef enum {
  FILE_READ_SUCCESS,
  FILE_READ_CANT_OPEN,
  FILE_READ_OUT_OF_MEMORY,
} FileReaderResult;


FileReaderResult readFile(const char* path, char** buffer);

#endif
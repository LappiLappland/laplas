#ifndef laplas_scanner_h
#define laplas_scanner_h

#include <stddef.h>

#include "globalState.h"

typedef enum {
  // ONE character
  TOKEN_ERROR, TOKEN_EOF,
  TOKEN_COMMA,  
  TOKEN_PLUS,
  TOKEN_STAR, TOKEN_PERCENT,
  TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
  TOKEN_LEFT_SQUARE, TOKEN_RIGHT_SQUARE,
  TOKEN_COLON, TOKEN_SEMICOLON,
  TOKEN_QUESTION, TOKEN_HASH,
  // ONE or TWO characters
  TOKEN_BANG, TOKEN_BANG_EQUAL,
  TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER, TOKEN_GREATER_EQUAL,
  TOKEN_LESS, TOKEN_LESS_EQUAL,
  TOKEN_SLASH, TOKEN_SLASH_SLASH,
  TOKEN_MINUS, TOKEN_MINUS_ARROW,
  // TWO characters
  TOKEN_AND, TOKEN_OR,
  // THREE characters
  //DOTs 
  TOKEN_DOT, //.
  TOKEN_DOT_DOT, //..
  TOKEN_DOT_DOT_EQUAL, //..=
  TOKEN_DOT_SPREAD, //...
  // Literals
  TOKEN_IDENTIFIER, TOKEN_STRING,
  TOKEN_NUMBER,
  // Keywords
  TOKEN_TRUE, TOKEN_FALSE,
  TOKEN_IF, TOKEN_ELSE,
  TOKEN_NULL, TOKEN_WHILE,
  TOKEN_FOR, TOKEN_IN,
  TOKEN_BREAK, TOKEN_CONTINUE,
  TOKEN_FUN, TOKEN_RETURN,
  TOKEN_THIS, TOKEN_SUPER,
  TOKEN_VAR, TOKEN_CONST,
  TOKEN_AS,
  // Types
  TOKEN_NUMBER_TYPE, TOKEN_STRING_TYPE
} TokenType;


typedef struct {
  TokenType type;
  const char* start;
  size_t length;
  size_t line;
} Token;

typedef struct {
  const char* start;
  const char* current;
  int line;
} Scanner;

void initScanner(Scanner* scanner, GlobalState* GS, const char* source);
Token scanToken(Scanner* scanner, GlobalState* GS);

#endif
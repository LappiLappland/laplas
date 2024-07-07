#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "common.h"
#include "scanner.h"
#include "object.h"

const char* const keywords[] = {
  "true", "false",
  "if", "else",
  "null", "while",
  "for", "in",
  "break", "continue",
  "fun", "return",
  "this", "super",
  "var", "const",
  "as",
  
  "Number", "String",
};

void initScanner(Scanner* scanner, GlobalState* GS, const char* source) {
  scanner->start = source;
  scanner->current = source;
  scanner->line = 1;

  for (size_t i = 0; i < sizeof(keywords) / sizeof(char*); i++) {
    char* chars = keywords[i];
    LString* str = newLString(GS, chars, strlen(chars));
    str->reservedId = i;
  }

#ifdef DEBUG_STRINGS
  printf("===== dumping scanner keywords =====\n");
  for (size_t i = 0; i < GS->LST.capacity; i++) {
    LString entry = GS->LST.entries[i];
    printf("%d -> %.*s \n", i, entry.length, entry.content);
  }
  printf("===== ======================== =====\n");
#endif
}

static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         (c == '_');
}

static bool isAtEnd(Scanner* scanner) {
  return *scanner->current == '\0';
}

static char advance(Scanner* scanner) {
  scanner->current++;
  return scanner->current[-1];
}

static char peek(Scanner* scanner) {
  return *scanner->current;
}

static char peekNext(Scanner* scanner) {
  if (isAtEnd(scanner)) return '\0';
  return scanner->current[1];
}

static bool match(Scanner* scanner, char want) {
  if (isAtEnd(scanner)) return false;
  if (*scanner->current != want) return false;
  scanner->current++;
  return true;
}

static Token initToken(Scanner* scanner, TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner->start;
  token.length = scanner->current - scanner->start;
  token.line = scanner->line;
  return token;
}

static Token initTokenError(Scanner* scanner, const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = scanner->start;
  token.length = strlen(message);
  token.line = scanner->line;
  return token;
}

static Token number(Scanner* scanner) {
  while (isDigit(peek(scanner)) || peek(scanner) == '_') advance(scanner);

  if (peek(scanner) == '.' && isDigit(peekNext(scanner))) {
    advance(scanner);

    while (isDigit(peek(scanner)) || peek(scanner) == '_') advance(scanner);
  }
  
  return initToken(scanner, TOKEN_NUMBER);
}

static Token string(Scanner* scanner, char opener) {
  while (peek(scanner) != opener && !isAtEnd(scanner)) {
    if (peek(scanner) == '\n') scanner->line++;
    advance(scanner);
  }

  if (isAtEnd(scanner)) return initTokenError(scanner, "Unterminated string");

  advance(scanner);
  return initToken(scanner, TOKEN_STRING);
}

//TODO: check for keywords
static TokenType getIdentifierType(Scanner* scanner, GlobalState* GS) {
  size_t length = scanner->current - scanner->start;
  
  LString* str = newLString(GS, scanner->start, length);

  //Found keyword
  if (str->reservedId != 0) {
    return (TokenType)(TOKEN_TRUE + str->reservedId);
  }

  //Found identifier
  return TOKEN_IDENTIFIER;
}

static Token identifier(Scanner* scanner, GlobalState* GS) {
  while (isAlpha(peek(scanner)) || isDigit(peek(scanner))) advance(scanner);
  return initToken(scanner, getIdentifierType(scanner, GS));
}

static void skipWhitespace(Scanner* scanner) {
  for (;;) {
    char c = peek(scanner);
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        advance(scanner);
        break;
      case '\n':
        advance(scanner);
        scanner->line++;
        break;
      //TODO: add block comments scanning
      case '/':
        if (peekNext(scanner) == '/') {
          while (peek(scanner) != '\n' && !isAtEnd(scanner)) advance(scanner);
        } else {
          return;
        }
        break;
      default:
        return;
    }
  }
}

Token scanToken(Scanner* scanner, GlobalState* GS) {
  skipWhitespace(scanner);
  scanner->start = scanner->current;

  if (isAtEnd(scanner)) return initToken(scanner, TOKEN_EOF);

  char c = advance(scanner);
  
  if (isDigit(c)) return number(scanner);
  if (isAlpha(c)) return identifier(scanner, GS);
  switch (c) {
    //ONE character
    case ',': return initToken(scanner, TOKEN_COMMA);
    case '+': return initToken(scanner, TOKEN_PLUS);
    case '*': return initToken(scanner, TOKEN_STAR);
    case '%': return initToken(scanner, TOKEN_PERCENT);
    case '(': return initToken(scanner, TOKEN_LEFT_PAREN);
    case ')': return initToken(scanner, TOKEN_RIGHT_PAREN);
    case '{': return initToken(scanner, TOKEN_LEFT_BRACE);
    case '}': return initToken(scanner, TOKEN_RIGHT_BRACE);
    case '[': return initToken(scanner, TOKEN_LEFT_SQUARE);
    case ']': return initToken(scanner, TOKEN_RIGHT_SQUARE);
    case ':': return initToken(scanner, TOKEN_COLON);
    case ';': return initToken(scanner, TOKEN_SEMICOLON);
    case '?': return initToken(scanner, TOKEN_QUESTION);
    case '#': return initToken(scanner, TOKEN_HASH);
    // ONE or TWO characters
    case '!': return (
      match(scanner, '=') ? initToken(scanner, TOKEN_BANG_EQUAL) : initToken(scanner, TOKEN_BANG)
    );
    case '=': return (
      match(scanner, '=') ? initToken(scanner, TOKEN_EQUAL_EQUAL) : initToken(scanner, TOKEN_EQUAL)
    );
    case '>': return (
      match(scanner, '=') ? initToken(scanner, TOKEN_GREATER_EQUAL) : initToken(scanner, TOKEN_GREATER)
    );
    case '<': return (
      match(scanner, '=') ? initToken(scanner, TOKEN_LESS_EQUAL) : initToken(scanner, TOKEN_LESS)
    );
    case '/': return (
      match(scanner, '/') ? initToken(scanner, TOKEN_SLASH_SLASH) : initToken(scanner, TOKEN_SLASH)
    );
    case '-': return (
      match(scanner, '>') ? initToken(scanner, TOKEN_MINUS_ARROW) : initToken(scanner, TOKEN_MINUS)
    );
    //TWO characters
    case '&': return (
      match(scanner, '&') ? initToken(scanner, TOKEN_AND) : initTokenError(scanner, "Unknown token \"&\", maybe wanted \"&&\"")
    );
    case '|': return (
      match(scanner, '|') ? initToken(scanner, TOKEN_OR) : initTokenError(scanner, "Unknown token \"|\", maybe wanted \"||\"")
    );
    //DOTs
    case '.': 
      if (match(scanner, '.')) {
        if (match(scanner, '.')) {
          return initToken(scanner, TOKEN_DOT_SPREAD);
        }
        if (match(scanner, '=')) {
          return initToken(scanner, TOKEN_DOT_DOT_EQUAL);
        }
        return initToken(scanner, TOKEN_DOT_DOT);
      }
      return initToken(scanner, TOKEN_DOT);
    //string
    case '"':
    case '\'':
      return string(scanner, c);
    //TODO: string with variables
    case '$':
    default:
      break;
  }

  //TODO: append incorrect token
  return initTokenError(scanner, "Unknown token");
}

#pragma once
#include <chaos.h>

typedef enum {
  TOKEN_EOF,
  TOKEN_PLUS,
  TOKEN_PLUS_EQ,
  TOKEN_MINUS,
  TOKEN_MINUS_EQ,
  TOKEN_DIVIDE,
  TOKEN_DIVIDE_EQ,
  TOKEN_MULT,
  TOKEN_MULT_EQ,
  TOKEN_NOT,
  TOKEN_NOT_EQ,
  TOKEN_LT,
  TOKEN_LEQ,
  TOKEN_GT,
  TOKEN_GEQ,
  TOKEN_ASSIGN,
  TOKEN_EQ,
  TOKEN_COLON,
  TOKEN_PAREN_OPEN,
  TOKEN_PAREN_CLOSE,
  TOKEN_BRACE_OPEN,
  TOKEN_BRACE_CLOSE,
  TOKEN_COMMA,
  TOKEN_DOT,
  TOKEN_END_OF_LINE,
  TOKEN_DIGIT,
  TOKEN_IDENTIFIER,
} token;

typedef struct scanner {
  u8 n;
  c8 ch;
  c8* it;
  c8* end;
} scanner;

extern void scanner_refill(scanner* self);

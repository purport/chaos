#pragma once
#include <chaos.h>

typedef enum {
  TOKEN_ERROR,
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
  TOKEN_NUMBER,
  TOKEN_IDENTIFIER,
  TOKEN_KEYWORD,
} token;

typedef enum {
  KEYWORD_AND = 7,
  KEYWORD_NAND = 1,
  KEYWORD_OR = 13,
  KEYWORD_NOR = 4,
  KEYWORD_XOR = 10,
  KEYWORD_NOT = 5,
  KEYWORD_FN = 6,
  KEYWORD_IF = 8,
  KEYWORD_ELSE = 14,
  KEYWORD_CONST = 2,
  KEYWORD_FOR = 11,
  KEYWORD_LET = 0,
  KEYWORD_TYPE = 9,
  KEYWORD_WHILE = 3,
  KEYWORD_RETURN = 12,
} keyword;

#define KEYWORD_COUNT 15

typedef struct scanner {
  u64 n;
  struct {
    u32 hash;
    c8* begin;
    c8* end;
  } ident;
  keyword keyword;
  c8* it;
  c8* end;
} scanner;

extern token scanner_next(scanner* self);

#include <scanner.h>
#include <chaos/hash.h>

// perfect hashes with seed 1065166
#define KEYWORD_AND_HASH 642211027
#define KEYWORD_NAND_HASH 1039042156
#define KEYWORD_OR_HASH 3311656483
#define KEYWORD_NOR_HASH 3926466829
#define KEYWORD_XOR_HASH 1030445155
#define KEYWORD_NOT_HASH 1355407970
#define KEYWORD_FN_HASH 1486930056
#define KEYWORD_IF_HASH 2458204898
#define KEYWORD_ELSE_HASH 2511306689
#define KEYWORD_CONST_HASH 2444039552
#define KEYWORD_FOR_HASH 3127266056
#define KEYWORD_LET_HASH 1414910715
#define KEYWORD_TYPE_HASH 407243829
#define KEYWORD_WHILE_HASH 205143348
#define KEYWORD_RETURN_HASH 3473396112
#define KEYWORD_MAX_LENGTH 6

static bool is_digit(c8 ch) { return '0' <= ch && ch <= '9'; }

static bool is_alpha(c8 ch) {
  return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

static bool is_alphanum(c8 ch) {
  return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || is_digit(ch);
}

static bool match(scanner *self, c8 ch) {
  if (self->it == self->end) return 0;
  if (*self->it != ch) return 0;
  ++self->it;
  return 1;
}

extern token scanner_next(scanner *self) {
  while (self->it != self->end) {
    c8 ch = *self->it++;
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') continue;
    if (ch == '+') return match(self, '=') ? TOKEN_PLUS_EQ : TOKEN_PLUS;
    if (ch == '-') return match(self, '=') ? TOKEN_MINUS_EQ : TOKEN_MINUS;
    if (ch == '/') return match(self, '=') ? TOKEN_DIVIDE_EQ : TOKEN_DIVIDE;
    if (ch == '*') return match(self, '=') ? TOKEN_MULT_EQ : TOKEN_MULT;
    if (ch == '!') return match(self, '=') ? TOKEN_NOT_EQ : TOKEN_NOT;
    if (ch == '<') return match(self, '=') ? TOKEN_LEQ : TOKEN_LT;
    if (ch == '>') return match(self, '=') ? TOKEN_GEQ : TOKEN_GT;
    if (ch == ':') return match(self, '=') ? TOKEN_ASSIGN : TOKEN_COLON;
    if (ch == '(') return TOKEN_PAREN_OPEN;
    if (ch == ')') return TOKEN_PAREN_CLOSE;
    if (ch == '{') return TOKEN_BRACE_OPEN;
    if (ch == '}') return TOKEN_BRACE_CLOSE;
    if (ch == ',') return TOKEN_COMMA;
    if (ch == '.') return TOKEN_DOT;
    if (ch == ';') return TOKEN_END_OF_LINE;
    if (ch == '=') return TOKEN_EQ;
    if (is_digit(ch)) {
      self->n = (u8)(ch - '0');
      for (; self->it != self->end; ++self->it) {
        ch = *self->it;
        if (!is_digit(ch)) break;
        bool overflow =
                __builtin_umulll_overflow(self->n, 10, &self->n) ||
                __builtin_uaddll_overflow(self->n, (u64)(ch - '0'), &self->n);
        if (overflow) {
          self->it = self->end;
          return TOKEN_ERROR;
        }
      }
      return TOKEN_NUMBER;
    }
    if (is_alpha(ch)) {
      murmur3 hash = {.value = 1065166};
      hash_murmur3(&hash, (u8)ch);
      self->ident.begin = self->it;
      for (; self->it != self->end; ++self->it) {
        ch = *self->it;
        if (!is_alphanum(ch)) break;
        hash_murmur3(&hash, (u8)ch);
      }
      self->ident.end = self->it;
      self->ident.hash = hash_murmur3_fin(&hash);
      if ((self->ident.end - self->it) <= KEYWORD_MAX_LENGTH) {

        switch (self->ident.hash) {
          case KEYWORD_AND_HASH:
          case KEYWORD_NAND_HASH:
          case KEYWORD_OR_HASH:
          case KEYWORD_NOR_HASH:
          case KEYWORD_XOR_HASH:
          case KEYWORD_NOT_HASH:
          case KEYWORD_FN_HASH:
          case KEYWORD_IF_HASH:
          case KEYWORD_ELSE_HASH:
          case KEYWORD_CONST_HASH:
          case KEYWORD_FOR_HASH:
          case KEYWORD_LET_HASH:
          case KEYWORD_TYPE_HASH:
          case KEYWORD_WHILE_HASH:
          case KEYWORD_RETURN_HASH:
            self->keyword = self->ident.hash % KEYWORD_COUNT;
            return TOKEN_KEYWORD;
        }
      }

      return TOKEN_IDENTIFIER;
    }
    // unexpected character
    self->it = self->end;
    return TOKEN_ERROR;
  }
  return TOKEN_EOF;
}

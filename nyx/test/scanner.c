#include <chaos/check.h>
#include <scanner.h>

static bool match(scanner *self, c8 ch) {
  if (self->it == self->end) return 0;
  if (*self->it != ch) return 0;
  ++self->it;
  return 1;
}

static bool is_digit(c8 ch) { return '0' <= ch && ch <= '9'; }
static bool is_alpha(c8 ch) {
  return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}
static bool is_alphanum(c8 ch) {
  return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || is_digit(ch);
}

static token scanner_next(scanner *self) {
  while (self->it != self->end) {
    c8 ch = *self->it++;
    if (ch == ' ' || ch == '\t') continue;
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

      return TOKEN_DIGIT;
    }
    if (is_alpha(ch)) {
      for (; self->it != self->end; ++self->it) {
        ch = *self->it;
        if (!is_alphanum(ch)) break;
      }
      return TOKEN_IDENTIFIER;
    }
  }
  return TOKEN_EOF;
}

static u64 buffer_chunk = 10;
static u64 buffer_index;
static u64 buffer_size;
static c8 buffer[1024];

static scanner fill_buffer(c8 *b) {
  buffer_index = 0;
  buffer_size = string_length(b);
  memcpy(buffer, b, buffer_size);
  return (scanner){.it = buffer, .end = buffer + buffer_size};
}

u32 chaos_main() {
  scanner s;
  CHECK_START();
  s = fill_buffer("");
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("           ");
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("+");
  CHECK(scanner_next(&s), ==, TOKEN_PLUS);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("0 1 2 3 4 5 6 7 8 9");
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 0);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 1);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 2);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 3);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 4);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 5);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 6);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 7);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 8);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 9);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("123 543");
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 123);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 543);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("18446744073709551615");
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(s.n, ==, 18446744073709551615ull);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("18446744073709551616");
  CHECK(scanner_next(&s), ==, TOKEN_ERROR);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("a+ asdf A0 0a");
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(scanner_next(&s), ==, TOKEN_PLUS);
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("*=::=.=<<=>>=0-1(){},;!!=+-*/+=-=*=/=");
  CHECK(scanner_next(&s), ==, TOKEN_MULT_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_COLON);
  CHECK(scanner_next(&s), ==, TOKEN_ASSIGN);
  CHECK(scanner_next(&s), ==, TOKEN_DOT);
  CHECK(scanner_next(&s), ==, TOKEN_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_LT);
  CHECK(scanner_next(&s), ==, TOKEN_LEQ);
  CHECK(scanner_next(&s), ==, TOKEN_GT);
  CHECK(scanner_next(&s), ==, TOKEN_GEQ);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(scanner_next(&s), ==, TOKEN_MINUS);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(scanner_next(&s), ==, TOKEN_PAREN_OPEN);
  CHECK(scanner_next(&s), ==, TOKEN_PAREN_CLOSE);
  CHECK(scanner_next(&s), ==, TOKEN_BRACE_OPEN);
  CHECK(scanner_next(&s), ==, TOKEN_BRACE_CLOSE);
  CHECK(scanner_next(&s), ==, TOKEN_COMMA);
  CHECK(scanner_next(&s), ==, TOKEN_END_OF_LINE);
  CHECK(scanner_next(&s), ==, TOKEN_NOT);
  CHECK(scanner_next(&s), ==, TOKEN_NOT_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_PLUS);
  CHECK(scanner_next(&s), ==, TOKEN_MINUS);
  CHECK(scanner_next(&s), ==, TOKEN_MULT);
  CHECK(scanner_next(&s), ==, TOKEN_DIVIDE);
  CHECK(scanner_next(&s), ==, TOKEN_PLUS_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_MINUS_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_MULT_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_DIVIDE_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  return CHECK_END();
}

#include <chaos/check.h>
#include <scanner.h>

static bool match(scanner *self, c8 ch) {
  if (self->it == self->end) scanner_refill(self);
  if (self->it == self->end) return 0;
  if (*self->it != ch) return 0;
  ++self->it;
  return 1;
}

static token scanner_next(scanner *self) {
  for (;;) {
    if (self->it == self->end) scanner_refill(self);
    if (self->it == self->end) return TOKEN_EOF;
    do {
      c8 ch = *self->it++;
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
      if ('0' <= ch && ch <= '9') {
        self->n = (u8)(ch - '0');
        return TOKEN_DIGIT;
      }
      if (ch == ' ') continue;
      if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z')) {
        while (self->it != self->end) {
          ch = *self->it++;
          if (ch == ' ' || ch == '\t') {
            break;
          }
          if (self->it == self->end) scanner_refill(self);
        }
        return TOKEN_IDENTIFIER;
      }
    } while (self->it != self->end);
  }
}

static u64 buffer_chunk = 10;
static u64 buffer_index;
static u64 buffer_size;
static c8 buffer[1024];

static void fill_buffer(c8 *b);

u32 chaos_main() {
  scanner s;
  CHECK_START();
  fill_buffer("");
  s = (scanner){0};
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  fill_buffer("           ");
  s = (scanner){0};
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  fill_buffer("+");
  s = (scanner){0};
  CHECK(scanner_next(&s), ==, TOKEN_PLUS);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  fill_buffer("0123456789");
  s = (scanner){0};
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
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  fill_buffer("asdf A0 0a");
  s = (scanner){0};
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  for (buffer_chunk = 1; buffer_chunk != 1; ++buffer_chunk) {
    fill_buffer(
            "*=::=.=<<=>>=0-123456789(){},;!!=+-*/+=-=*=/="
            "*=::=.=<<=>>=0-123456789(){},;!!=+-*/+=-=*=/="
            "*=::=.=<<=>>=0-123456789(){},;!!=+-*/+=-=*=/="
            "*=::=.=<<=>>=0-123456789(){},;!!=+-*/+=-=*=/="
            "*=::=.=<<=>>=0-123456789(){},;!!=+-*/+=-=*=/="
            "*=::=.=<<=>>=0-123456789(){},;!!=+-*/+=-=*=/="
            "*=::=.=<<=>>=0-123456789(){},;!!=+-*/+=-=*=/="
            "*=::=.=<<=>>=0-123456789(){},;!!=+-*/+=-=*=/="
            "*=::=.=<<=>>=0-123456789(){},;!!=+-*/+=-=*=/="
            "*=::=.=<<=>>=0-123456789(){},;!!=+-*/+=-=*=/=");
    s = (scanner){0};
    for (u8 i = 0; i != 10; ++i) {
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
      CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
      CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
      CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
      CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
      CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
      CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
      CHECK(scanner_next(&s), ==, TOKEN_DIGIT);
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
    }
    CHECK(scanner_next(&s), ==, TOKEN_EOF);
    CHECK(s.it, ==, buffer + buffer_size);
    CHECK(s.end, ==, buffer + buffer_size);
  }

  return CHECK_END();
}

extern void scanner_refill(scanner *self) {
  static bool log_chunks = 1;
  ASSERT(self->it == self->end);
  self->it = buffer + buffer_index;
  buffer_index = min_u64(buffer_index + buffer_chunk, buffer_size);
  self->end = buffer + buffer_index;

  if (log_chunks) {
    c8 ch = *self->end;
    print("CHUNK: '");
    *self->end = 0;
    print(self->it);
    *self->end = ch;
    print("'\n");
  }
}

static void fill_buffer(c8 *b) {
  buffer_index = 0;
  buffer_size = string_length(b);
  memcpy(buffer, b, buffer_size);
}

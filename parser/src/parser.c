#include <chaos.h>
#define ALWAYS_INLINE inline __attribute__((__always_inline__))

typedef struct input_t {
  u64 length;
  c8 *input;
} input_t;

typedef union match_t {
  u64 literal;
  c8 ch;
} match_t;

typedef struct combinator_t combinator_t;

typedef union parser_t {
  match_t match;
  struct {
    combinator_t (*f)(match_t match);
    combinator_t *p;
  } bind;
} parser_t;

typedef struct result_t {
  bool success;
  match_t match;
  input_t rest;
} result_t;

typedef struct combinator_t {
  result_t (*parser)(parser_t, input_t);
  parser_t self;
} combinator_t;

static ALWAYS_INLINE result_t result(parser_t self, input_t input) {
  return (result_t){.success = 1, .match = self.match, .rest = input};
}

static ALWAYS_INLINE combinator_t char_to_number(match_t m) {
  return (combinator_t){result, {.match = {.literal = (u64)(m.ch - '0')}}};
}

static ALWAYS_INLINE result_t bind(parser_t self, input_t input) {
  result_t r = self.bind.p->parser(self.bind.p->self, input);
  if (r.success) {
    combinator_t p2 = self.bind.f(r.match);
    return p2.parser(p2.self, r.rest);
  }
  return r;
}

u32 chaos_main() {
  input_t input;
  input.length = 0;
  combinator_t zero_to_nine = {
  combinator_t digit = {bind,
                        {.bind = {.f = char_to_number, .p = &zero_to_nine}}};

  print_u64(one.parser(one.self, input).match.literal);

  return 0;
}

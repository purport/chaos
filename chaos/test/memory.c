#include <chaos/check.h>

#define BITS_PER_STATE 6

#define TOKEN_LESS          1 * BITS_PER_STATE
#define TOKEN_LESS_EQUAL    2 * BITS_PER_STATE
#define TOKEN_SHIFT_LEFT    3 * BITS_PER_STATE
#define TOKEN_GREATER       4 * BITS_PER_STATE
#define TOKEN_GREATER_EQUAL 5 * BITS_PER_STATE
#define TOKEN_SHIFT_RIGHT   6 * BITS_PER_STATE
#define TOKEN_BANG          7 * BITS_PER_STATE
#define TOKEN_BANG_EQUAL    8 * BITS_PER_STATE

static void print_state(u64 state) {
  switch (state) {
    case TOKEN_LESS:
      print("TOKEN_LESS");
      break;
    case TOKEN_LESS_EQUAL:
      print("TOKEN_LESS_EQUAL");
      break;
    case TOKEN_SHIFT_LEFT:
      print("TOKEN_SHIFT_LEFT");
      break;
    case TOKEN_GREATER:
      print("TOKEN_GREATER");
      break;
    case TOKEN_GREATER_EQUAL:
      print("TOKEN_GREATER_EQUAL");
      break;
    case TOKEN_SHIFT_RIGHT:
      print("TOKEN_SHIFT_RIGHT");
      break;
    case TOKEN_BANG:
      print("TOKEN_BANG");
      break;
    case TOKEN_BANG_EQUAL:
      print("TOKEN_BANG_EQUAL");
      break;
  }
}

static u64 scanner(c8 *it, c8 *end, u64 state) {
  static u64 states[256] = {
          //                  999999888888777777666666555555444444333333222222111111
          ['<'] = 0b0000000000000000000000000000000000000000000000000000000011000001,
          ['>'] = 0b0000000000000000000000000000000000000110000000000000000000000100,
          ['!'] = 0b0000000000000000000000000000000000000000000000000000000000000111,
          ['='] = 0b0000000000000000001000000000000000000101000000000000000010000001,
          ['+'] = 0b0000000000000000001000000000000000000101000000000000000010000001,
  };
  static bool initialized = 0;
  if (!initialized) {
    initialized = 1;
    for (int i = 0; i != 256; ++i) states[i] *= BITS_PER_STATE;
  }
  while (it != end) {
    u64 row = states[(u8)*it];
    state = row >> (state & 63);
    print_state(state & 63);
    print("\n");
    ++it;
  }
  return state & 63;
}

u32 chaos_main() {
  c8 *input = "< <= << > >= >> ! !=";
  u64 state = 0;
  state = scanner(input, &input[string_length(input)], state);

  CHECK_START();
  u8 *buffer = 0;
  CHECK(memory_realloc(0, 0, 0), ==, 0);
  CHECK(memory_realloc(0, 1, 0), ==, 0);
  CHECK(buffer = memory_realloc(0, 0, 1), !=, 0);
  buffer[0] = 1;
  CHECK(buffer[0], ==, 1);
  CHECK(buffer = memory_realloc(buffer, 1, 2), !=, 0);
  buffer[1] = 2;
  CHECK(buffer[0], ==, 1);
  CHECK(buffer[1], ==, 2);
  CHECK(memory_realloc(buffer, 2, 0xffffffffff), ==, 0);
  CHECK(buffer[0], ==, 1);
  CHECK(buffer[1], ==, 2);
  CHECK(buffer = memory_realloc(buffer, 2, 1), !=, 0);
  CHECK(buffer[0], ==, 1);
  CHECK(buffer = memory_realloc(buffer, 1, 0), ==, 0);
  return CHECK_END();
}

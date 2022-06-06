#include <chaos/check.h>

u32 chaos_main() {
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

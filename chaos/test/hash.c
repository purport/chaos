#include <chaos/check.h>
#include <chaos/hash.h>

u32 chaos_main() {
  u32 h;
  murmur3 m;
  CHECK_START();
  m = (murmur3){.value = 0};
  CHECK(hash_murmur3_fin(&m), ==, 0);
  m = (murmur3){.value = 1};
  CHECK(hash_murmur3_fin(&m), ==, 0x514E28B7);
  m = (murmur3){.value = 0xffffffff};
  CHECK(hash_murmur3_fin(&m), ==, 0x81F16F39);

  m = (murmur3){.value = 0x0};
  hash_murmur3(&m, 0xff);
  hash_murmur3(&m, 0xff);
  hash_murmur3(&m, 0xff);
  hash_murmur3(&m, 0xff);
  CHECK(m.k_shift, ==, 0);
  CHECK(m.size, ==, 4);
  CHECK(h = hash_murmur3_fin(&m), ==, 0x76293B50);

  m = (murmur3){.value = 0x0};
  hash_murmur3(&m, 0x21);
  hash_murmur3(&m, 0x43);
  hash_murmur3(&m, 0x65);
  hash_murmur3(&m, 0x87);
  CHECK(h = hash_murmur3_fin(&m), ==, 0xF55B516B);

  m = (murmur3){.value = 0x5082EDEE};
  hash_murmur3(&m, 0x21);
  hash_murmur3(&m, 0x43);
  hash_murmur3(&m, 0x65);
  hash_murmur3(&m, 0x87);
  CHECK(h = hash_murmur3_fin(&m), ==, 0x2362F9DE);

  m = (murmur3){.value = 0x0};
  hash_murmur3(&m, 0x21);
  hash_murmur3(&m, 0x43);
  hash_murmur3(&m, 0x65);
  CHECK(h = hash_murmur3_fin(&m), ==, 0x7E4A8634);

  m = (murmur3){.value = 0x0};
  hash_murmur3(&m, 0x21);
  hash_murmur3(&m, 0x43);
  CHECK(h = hash_murmur3_fin(&m), ==, 0xA0F7B07A);

  m = (murmur3){.value = 0x0};
  hash_murmur3(&m, 0x21);
  CHECK(h = hash_murmur3_fin(&m), ==, 0x72661CF4);

  return CHECK_END();
}

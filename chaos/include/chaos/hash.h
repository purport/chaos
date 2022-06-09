#pragma once

#include <chaos.h>

typedef struct murmur3 {
  u32 value;
  u32 size;
  u32 k;
  u8 k_shift;
} murmur3;

static void hash_murmur3(murmur3 *self, u8 ch) {
  static u32 c1 = 0xcc9e2d51;
  static u32 c2 = 0x1b873593;
  static u32 m = 5;
  static u32 n = 0xe6546b64;
  ++self->size;
  self->k |= (u32)(ch << self->k_shift);
  self->k_shift += 8;
  if (self->k_shift == 32) {
    self->k *= c1;
    self->k = (self->k << 15) | (self->k >> 17);
    self->k *= c2;

    self->value ^= self->k;
    self->value = (self->value << 13) | (self->value >> 19);
    self->value *= m;
    self->value += n;

    self->k_shift = 0;
    self->k = 0;
  }
}

static u32 hash_murmur3_fin(murmur3 *self) {
  static u32 c1 = 0xcc9e2d51;
  static u32 c2 = 0x1b873593;
  u32 value = self->value;
  if (self->k_shift != 0) {
    u32 k = self->k * c1;
    k = (k << 15) | (k >> 17);
    k *= c2;
    value ^= k;
  }

  value ^= self->size;
  value ^= value >> 16;
  value *= 0x85ebca6b;
  value ^= value >> 13;
  value *= 0xc2b2ae35;
  value ^= value >> 16;
  return value;
}

#include <chaos.h>

extern void* memset(void* ptr, u32 value, u64 size) {
  unsigned char* it = &((unsigned char*)ptr)[0];
  while (size != 0) {
    *it = (unsigned char)value;
    ++it;
    --size;
  }
  return ptr;
}

extern void* memcpy(void* destination, void const* source, u64 size) {
  unsigned char const* src = source;
  unsigned char* dst = destination;
  while (size != 0) {
    *dst = *src;
    ++dst;
    ++src;
    --size;
  }
  return destination;
}


#include <chaos.h>

extern bool next_permutation(u64 length, u32 array[]) {
  // Find non-increasing suffix
  if (length == 0) return 0;

  u64 i = length - 1;
  while (i > 0 && array[i - 1] >= array[i]) i--;
  if (i == 0) return 0;

  // Find successor to pivot
  u64 j = length - 1;
  while (array[j] <= array[i - 1]) j--;

  swap_u32(&array[i - 1], &array[j]);

  // Reverse suffix
  j = length - 1;
  while (i < j) {
    swap_u32(&array[i], &array[j]);
    i++;
    j--;
  }
  return 1;
}

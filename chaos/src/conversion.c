#include <chaos.h>

extern bool u64_to_string(u64 num, u64 length, c8 *result, u64 *number_length) {
  c8 buffer[20];
  u64 max_length = length > 20 ? 20 : (length - 1);
  u64 len = 0;
  do {
    u64 unit = num % 10;
    num = (num - unit) / 10;
    buffer[len++] = '0' + (c8)unit;
    if (len == max_length) break;
  } while (num != 0);

  u64 j = 0;
  for (u64 i = len - 1; j != len; ++j, --i) {
    result[j] = buffer[i];
  }

  result[j] = '\0';
  if (number_length) *number_length = j;
  return num == 0;
}

extern bool string_to_u64(u64 length, c8 *buffer, u64 *num) {
  i64 sign = buffer[0] == '-' ? -1 : 1;
  u64 n = 0;
  for (u64 i = (sign == -1 ? 1 : 0); (i != length); ++i) {
    u64 next;
    u64 unit = (u64)buffer[i] - '0';
    bool overflowed = __builtin_umulll_overflow(n, 10, &next) ||
                      __builtin_uaddll_overflow(next, unit, &n);
    if (overflowed) return 0;
  }
  *num = sign == -1 ? ~n : n;
  return 1;
}

extern bool f64_to_string(f64 num, u64 length, c8 *result, u64 *number_length) {
  u64 l;
  if (num < 0) {
    num *= -1;
    result[0] = '-';
    ++result;
    --length;
  }
  u64 dec = (u64)num;
  if (!u64_to_string(dec, length, result, &l)) return 0;
  u64 frac = (u64)((num - (f64)dec) * 10000);
  result[l++] = '.';
  if (frac < 10) result[l++] = '0';
  if (frac < 100) result[l++] = '0';
  if (frac < 1000) result[l++] = '0';
  return u64_to_string(frac, length - l, result + l, &l);
}

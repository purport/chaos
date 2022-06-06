#pragma once
#include <chaos.h>

static u32 check_end(u32 failed, u32 total) {
  if (failed) {
    print("[FAIL] ");
    print_u64(failed);
    print("/");
    print_u64(total);
  } else {
    print("[PASS] ");
    print_u64(total);
  }
  print(" tests");
  print("\n");

  return failed;
}

#define CHECK_START() \
  u32 failed = 0;     \
  u32 total = 0
#define CHECK_END() check_end(failed, total)

#define CHECK(lhs, op, rhs)                            \
  do {                                                 \
    ++total;                                           \
    if ((lhs)op(rhs)) {                                \
      /*print("[PASS] " #lhs " " #op " " #rhs "\n");*/ \
    } else {                                           \
      print("[FAIL] " #lhs " " #op " " #rhs "\n");     \
      ++failed;                                        \
    }                                                  \
  } while (0)

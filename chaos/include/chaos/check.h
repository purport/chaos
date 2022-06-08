#pragma once
#include <chaos.h>

static u32 check_end(u32 failed, u32 total) {
  if (failed) {
    print("\x1b[31m[FAIL]\x1b[0m ");
    print_u64(failed);
    print("/");
    print_u64(total);
  } else {
    print("\x1b[32m[PASS]\x1b[0m ");
    print_u64(total);
  }
  print(" tests");
  print("\n");

  return failed;
}

#define CHECK_START() \
  u32 failed = 0;     \
  u32 total = 0
#define CHECK_END()   check_end(failed, total)
#define STRINGIZE(x)  STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING   STRINGIZE(__LINE__)

#define CHECK(lhs, op, rhs)                                                \
  do {                                                                     \
    ++total;                                                               \
    if ((lhs)op(rhs)) {                                                    \
      /*print("[PASS] " #lhs " " #op " " #rhs "\n");*/                     \
    } else {                                                               \
      print("\x1b[31m[FAIL]\x1b[0m " #lhs " " #op " " #rhs " " LINE_STRING \
            "\n");                                                         \
      ++failed;                                                            \
    }                                                                      \
  } while (0)

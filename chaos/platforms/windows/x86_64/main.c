#include "windows.h"

#define STD_INPUT_HANDLE  ((u32)-10)
#define STD_OUTPUT_HANDLE ((u32)-11)
#define STD_ERROR_HANDLE  ((u32)-12)

IMPORT HANDLE WINAPI GetStdHandle(DWORD nStdHandle);
IMPORT BOOL WINAPI WriteConsoleA(HANDLE hConsoleOutput, const VOID* lpBuffer,
                                 DWORD nNumberOfCharsToWrite,
                                 LPDWORD lpNumberOfCharsWritten,
                                 LPVOID lpReserved);

extern void print(c8* message) {
  u64 len = string_length(message);
  // TODO: maybe bail here on large int.
  WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (u32)len, 0, 0);
}

IMPORT _Noreturn void WINAPI ExitProcess(UINT uExitCode);

extern int _fltused;
int _fltused = 0;
extern _Noreturn void mainCRTStartup(void);
extern _Noreturn void mainCRTStartup() {
  ExitProcess(chaos_main());
}

#include <chaos.h>

// https://source.chromium.org/chromium/chromium/src/+/main:base/win/windows_types.h
typedef void* HANDLE;
typedef unsigned long DWORD;  // NOLINT(runtime/int)
typedef DWORD* LPDWORD;       // NOLINT(runtime/int)
typedef long LONG;            // NOLINT(runtime/int)
typedef i64 LONGLONG;
typedef u64 ULONGLONG;

#define VOID void
typedef char CHAR;
typedef short SHORT;  // NOLINT(runtime/int)
typedef long LONG;    // NOLINT(runtime/int)
typedef int INT;
typedef unsigned int UINT;
typedef unsigned int* PUINT;
typedef u64 UINT64;
typedef void* LPVOID;
typedef void* PVOID;
typedef void* HANDLE;
typedef int BOOL;
typedef unsigned char BYTE;
typedef BYTE BOOLEAN;
typedef DWORD ULONG;
typedef unsigned short WORD;  // NOLINT(runtime/int)
typedef WORD UWORD;
typedef WORD ATOM;

#define STD_INPUT_HANDLE  ((u32)-10)
#define STD_OUTPUT_HANDLE ((u32)-11)
#define STD_ERROR_HANDLE  ((u32)-12)

#define WINAPI __stdcall
#define IMPORT __declspec(dllimport)

IMPORT _Noreturn void WINAPI ExitProcess(UINT uExitCode);

IMPORT HANDLE WINAPI GetStdHandle(DWORD nStdHandle);
IMPORT BOOL WINAPI AllocConsole(void);
IMPORT BOOL WINAPI WriteConsoleA(HANDLE hConsoleOutput, const VOID* lpBuffer,
                                 DWORD nNumberOfCharsToWrite,
                                 LPDWORD lpNumberOfCharsWritten,
                                 LPVOID lpReserved);

extern u32 string_length(c8* message) {
  u32 len = 0;
  while (message[len] != '\0') ++len;
  return len;
}

extern void print(c8* message) {
  u32 len = string_length(message);
  WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, len, 0, 0);
}

extern _Noreturn void mainCRTStartup(void);
extern _Noreturn void mainCRTStartup() { ExitProcess(chaos_main()); }

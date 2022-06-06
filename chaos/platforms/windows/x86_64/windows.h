#pragma once
#include <chaos.h>

#pragma comment(lib, "kernel32.lib")

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

typedef i64 INT_PTR, *PINT_PTR;
typedef u64 UINT_PTR, *PUINT_PTR;

typedef i64 LONG_PTR, *PLONG_PTR;
typedef u64 ULONG_PTR, *PULONG_PTR;

typedef ULONG_PTR SIZE_T, *PSIZE_T;
typedef LONG_PTR SSIZE_T, *PSSIZE_T;
#define WINAPI __stdcall
#define IMPORT __declspec(dllimport)


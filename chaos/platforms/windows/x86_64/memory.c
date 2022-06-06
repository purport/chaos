#include "windows.h"

#define PAGE_NOACCESS          0x01
#define PAGE_READONLY          0x02
#define PAGE_READWRITE         0x04
#define PAGE_WRITECOPY         0x08
#define PAGE_EXECUTE           0x10
#define PAGE_EXECUTE_READ      0x20
#define PAGE_EXECUTE_READWRITE 0x40
#define PAGE_EXECUTE_WRITECOPY 0x80
#define PAGE_GUARD             0x100
#define PAGE_NOCACHE           0x200
#define PAGE_WRITECOMBINE      0x400

#define MEM_COMMIT                     0x00001000
#define MEM_RESERVE                    0x00002000
#define MEM_REPLACE_PLACEHOLDER        0x00004000
#define MEM_RESERVE_PLACEHOLDER        0x00040000
#define MEM_RESET                      0x00080000
#define MEM_TOP_DOWN                   0x00100000
#define MEM_WRITE_WATCH                0x00200000
#define MEM_PHYSICAL                   0x00400000
#define MEM_ROTATE                     0x00800000
#define MEM_DIFFERENT_IMAGE_BASE_OK    0x00800000
#define MEM_RESET_UNDO                 0x01000000
#define MEM_LARGE_PAGES                0x20000000
#define MEM_4MB_PAGES                  0x80000000
#define MEM_64K_PAGES                  (MEM_LARGE_PAGES | MEM_PHYSICAL)
#define MEM_UNMAP_WITH_TRANSIENT_BOOST 0x00000001
#define MEM_COALESCE_PLACEHOLDERS      0x00000001
#define MEM_PRESERVE_PLACEHOLDER       0x00000002
#define MEM_DECOMMIT                   0x00004000
#define MEM_RELEASE                    0x00008000
#define MEM_FREE                       0x00010000

LPVOID VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType,
                    DWORD flProtect);
BOOL VirtualFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);

extern void* memory_realloc(void* address, u64 prev, u64 new) {
  ASSERT(address == 0 || prev != 0);
  if (new == 0) {
    VirtualFree(address, 0, MEM_RELEASE);
    return 0;
  }

  void* mem = VirtualAlloc(0, new, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (address && mem) {
    memcpy(mem, address, prev <= new ? prev : new);
    VirtualFree(address, 0, MEM_RELEASE);
  }

  return mem;
}

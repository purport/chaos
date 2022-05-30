filter { "toolset:clang", "system:windows" }
  buildoptions { "-Weverything -Werror -Wno-unused-macros -Wno-newline-eof -Wno-unused-parameter -Wno-unused-function", "-ffreestanding", "-nostdinc", "-fno-builtin", "-mno-stack-arg-probe"}
  linkoptions { "-lkernel32", "-fuse-ld=lld", "-nodefaultlibs", "-nostdlib", "-Xlinker /subsystem:console" }
filter {}

project "chaos"
  kind "StaticLib"
  language "C"
  includedirs { "include" }
  files { "src/*.c" }
  filter { "system:windows" }
    files { "platforms/windows/x86_64/*.c" }
  filter {}

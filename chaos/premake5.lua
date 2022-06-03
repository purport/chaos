filter { "toolset:clang", "system:windows" }
  buildoptions { "-g -O0", "-Weverything -Werror -Wno-reserved-identifier -Wno-unused-macros -Wno-newline-eof -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-declaration-after-statement", "-ffreestanding", "-nostdinc", "-fno-builtin", "-mno-stack-arg-probe"}
  linkoptions { "-g -O0", "-fuse-ld=lld", "-nodefaultlibs", "-nostdlib", "-Xlinker /subsystem:console" }
filter {}

project "chaos"
  kind "StaticLib"
  language "C"
  includedirs { "include" }
  files { "src/*.c" }
  filter { "system:windows" }
    files { "platforms/windows/x86_64/*.c" }
  filter {}

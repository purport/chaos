
project "nyx-test-scanner"
  kind "ConsoleApp"
  language "C"
  includedirs { "include" }
  files { "src/scanner.c", "test/scanner.c" }
  links { "chaos" }

project "nyx"
  kind "ConsoleApp"
  language "C"
  includedirs { "include" }
  files { "src/*.c" }
  links { "chaos" }

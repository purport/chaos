require "ninja"
require "export-compile-commands"


workspace "Project"
  location ".build"
  configurations { "debug", "release" }
  targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
  filter "configurations:debug"
    defines { "DEBUG" }
    symbols "On"
  filter {}
  filter "configurations:release"
    defines { "NDEBUG" }
    optimize "On"
  filter {}

  include "chaos"

  project "game-theory"
    kind "ConsoleApp"
    language "C"
    files { "game_theory/src/shapley_shubik.c" }
    links { "chaos" }
    includedirs { "chaos/include" }


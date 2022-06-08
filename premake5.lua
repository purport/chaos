require "ninja"
require "export-compile-commands"

workspace "Project"
  location ".build"
  configurations { "debug", "release" }
  targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
  filter "configurations:debug"
    defines { "DEBUG" }
    symbols "On"
    optimize "On"
  filter {}
  filter "configurations:release"
    defines { "NDEBUG" }
    optimize "On"
  filter {}
  includedirs { "chaos/include" }

  include "chaos"
  include "nyx"
  include "maths"


project "shapley-shubik"
  kind "ConsoleApp"
  language "C"
  files { "src/shapley_shubik.c" }
  links { "chaos" }

project "lp"
  kind "ConsoleApp"
  language "C"
  files { "src/lp.c" }
  links { "chaos" }

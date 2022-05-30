premake5 --cc=clang ninja
premake5 --cc=clang export-compile-commands
cp .build\compile_commands\debug.json compile_commands.json

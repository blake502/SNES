@echo off

echo Building...

SetLocal EnableDelayedExpansion

set input_files=
for /R %%f in (*.c) do ( set input_files=!input_files! %%f )

SetLocal DisableDelayedExpansion

set linker_flags=-luser32
set include_flags=
set compiler_flags=-g

set output_dir=bin\
set output_name=main
set output_type=.exe
set output_path=%output_dir%%output_name%%output_type%

if not exist %output_dir% mkdir %output_dir%

clang %input_files% %linker_flags% %include_flags% %compiler_flags% -o%output_path%

echo Building complete!
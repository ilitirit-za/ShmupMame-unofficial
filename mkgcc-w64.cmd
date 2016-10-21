@echo off

set MINGW_ROOT=C:\msys64\mingw64
set PATH=%MINGW_ROOT%\bin;%PATH%

gcc -v

make >compile.log
pause

@echo off

set ORIGINAL_PATH=%PATH%
set SUBTARGET=mame
set SUFFIX=
set PREFIX="shmup"

set COMMONFLAGS=SUBTARGET=%SUBTARGET% MSVC_BUILD=1 MAXOPT= PREFIX=%PREFIX% SUFFIX=%SUFFIX% WINUI=
set MINGW_ROOT=C:\MinGW\mingw-w64\x86_64-5.4.0-win32-seh-rt_v5-rev0\mingw64
set USR_ROOT=%MINGW_ROOT%\..\usr
set PSDK_DIR=%ProgramFiles%\Microsoft Visual Studio 12.0\VC\
set INCLUDE=extravc\include\;%PSDK_DIR%\Include\
set LIB=extravc\lib\;extravc\lib\x64;%PSDK_DIR%\Lib\
set PATH=%PSDK_DIR%\bin\;%MINGW_ROOT%\bin;%USR_ROOT%\bin;%PATH%

call "%VS140COMNTOOLS%\..\..\VC\bin\amd64\vcvars64.bat"

gcc -v

make %COMMONFLAGS% maketree obj/windows/%PREFIX%mame%SUFFIX%64/build/vconv.exe
make %COMMONFLAGS% NO_FORCEINLINE=1 obj/windows/%PREFIX%mame%SUFFIX%64/emu/cpu/m6809/m6809.o
make %COMMONFLAGS% NO_FORCEINLINE=1 obj/windows/%PREFIX%mame%SUFFIX%64/emu/cpu/mips/mips3drc.o
make %COMMONFLAGS% OPTIMIZE=ng obj/windows/%PREFIX%mame%SUFFIX%64/emu/mconfig.o

make %COMMONFLAGS% 

set PATH=%ORIGINAL_PATH%
set ORIGINAL_PATH=
pause

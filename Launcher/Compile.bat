@echo off
setlocal

set VS_VCVARS="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
set PROJECT_DIR=%~dp0

call %VS_VCVARS% || exit /b 1
cd /d "%PROJECT_DIR%" || exit /b 1

rc /nologo icon.rc || exit /b 1

cl Main.cpp icon.res ^
 /EHsc- ^
 /O2 ^
 /Ob3 ^
 /Oi ^
 /Ot ^
 /GL ^
 /Gw ^
 /Gy ^
 /GS- ^
 /GR- ^
 /D NDEBUG ^
 /link ^
 /LTCG ^
 /OPT:REF ^
 /OPT:ICF ^
 /INCREMENTAL:NO ^
 /SUBSYSTEM:CONSOLE ^
 /OUT:"%PROJECT_DIR%\bin\launcher.exe" ^
 /MACHINE:X64 ^
 user32.lib

pause

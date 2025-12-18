@echo off
setlocal

set VS_VCVARS="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
set STEAM_SDK=C:\SteamworksSDK
set PROJECT_DIR=%~dp0

call %VS_VCVARS% || exit /b 1
cd /d "%PROJECT_DIR%" || exit /b 1

cl Main.cpp ^
 /LD ^
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
 /I "%STEAM_SDK%\public" ^
 /I "%STEAM_SDK%\public\steam" ^
 /link ^
 /LTCG ^
 /OPT:REF ^
 /OPT:ICF ^
 /INCREMENTAL:NO ^
 /OUT:"%PROJECT_DIR%\bin\RConnectMod.dll" ^
 /MACHINE:X64 ^
 "%STEAM_SDK%\redistributable_bin\win64\steam_api64.lib"

pause

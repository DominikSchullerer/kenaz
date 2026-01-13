REM ==============================
REM Build script for game (DLL)
REM ==============================
@ECHO OFF
SetLocal EnableDelayedExpansion

echo Files:
SET cFilenames=
FOR /R %%f IN (*.c) DO (
    SET cFilenames=!cFilenames! %%f
    echo     %%f
)

SET assembly=testbed
SET compilerFlags=-g -shared -Wall -Werror
SET includeFlags=-Isrc -I../engine/src
SET defines=-D_DEBUG -DK_GAME_DLL

echo Building %assembly%.dll ...
clang %cFilenames% %compilerFlags% -o ../bin/%assembly%.dll %defines% %includeFlags%

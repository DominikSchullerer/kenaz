REM ==============================
REM Build script for engine (EXE)
REM ==============================
@ECHO OFF
SetLocal EnableDelayedExpansion

echo Files:
SET cFilenames=
FOR /R %%f IN (*.c) DO (
    SET cFilenames=!cFilenames! %%f
    echo     %%f
)

SET assembly=kenaz
SET compilerFlags=-g -Wall -Werror
SET includeFlags=-Isrc
SET linkerFlags=-luser32
SET defines=-D_DEBUG -DK_ENGINE -D_CRT_SECURE_NO_WARNINGS

echo Building %assembly%.exe ...
clang %cFilenames% %compilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%

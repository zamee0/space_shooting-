@echo off
setlocal

:: Set default source file if not provided
if "%~1"=="" (
    set "SOURCE_FILE=iMain.cpp"
    echo No input file specified. Using default: iMain.cpp
) else (
    set "SOURCE_FILE=%~1"
)

:: Set the base directory for g++ (optional if g++ is in your PATH)
set "BASE_DIR=%~dp0MINGW"
set "PATH=%BASE_DIR%;%BASE_DIR%\bin;%PATH%"

echo Using g++ from: %BASE_DIR%

:: Create obj directory if it doesn't exist
if not exist "obj" (
    mkdir obj
)
:: Compile the source file to an object file (with irrKlang include path)

g++.exe -w -fexceptions -g -I. -IOpenGL\\include -IOpenGL\\include\\SDL2 -IOpenGL\\include\\Freetype -c "%SOURCE_FILE%" -o obj\\opengl.o

if %ERRORLEVEL% neq 0 (
   echo Compilation failed.
   exit /b 1
)

echo Compiling %SOURCE_FILE% to object file...

g++.exe -static-libgcc -static-libstdc++ -L.\\OpenGL\\lib -o bin\\opengl.exe obj\\opengl.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lOPENGL32 -lfreeglut -lwinmm -lfreetype

if %ERRORLEVEL% neq 0 (
    echo Linking failed.
    exit /b 1
)

echo Linking %SOURCE_FILE% to executable...

echo Finished building.

set "OUTPUT_EXE=opengl.exe"
:: Run the executable
echo Running bin\%OUTPUT_EXE%...
bin\%OUTPUT_EXE%

:: Check for runtime errors
if %ERRORLEVEL% neq 0 (
    echo Error occurred while running %OUTPUT_EXE% ^(exit code: %ERRORLEVEL%^)
    exit /b %ERRORLEVEL%
)
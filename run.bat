@echo off
REM 删除 build 文件夹及其内容
rmdir /s /q build

REM 删除当前目录下的 output.exe
del output.exe

REM 运行 CMake，生成 MinGW Makefiles
cmake -B "build" -G "MinGW Makefiles"

REM 生成在build
mingw32-make -C build

REM 运行程序 
"./output.exe"

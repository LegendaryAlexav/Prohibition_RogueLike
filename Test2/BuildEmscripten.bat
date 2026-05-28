rem Emscripten expected to be installed to `c:\emsdk`
rem     Info) https://emscripten.org
rem     Download) clone https://github.com/emscripten-core/emsdk.git to c:\emsdk
rem     Install) Run: emsdk.bat install latest
rem              Run: emsdk.bat activate latest
rem              Run: emsdk_env.bat
rem Ninja must be installed to c:\Apps\ninja
rem     Info) https://ninja-build.org/
rem     Download) https://github.com/ninja-build/ninja/releases
rem Node needs installing for running a local web server
rem     Info) https://nodejs.org
rem     Download) https://nodejs.org/en/download

path=%path%;c:\Apps\ninja

set EMSCRIPTEN=c:\emsdk

call %EMSCRIPTEN%\emsdk_env.bat

rem Create symlinks for the source folders, only to allow for source maps while debugging.
mkdir build-em\debug
mklink /J "build-em\debug\Game" "Game"
mklink /J "build-em\debug\Framework" "Framework"

echo --------------------------------------------------------------------------
echo ---------------------------- Building Debug ----------------------------
echo --------------------------------------------------------------------------
call emcmake cmake -S . -B build-em\debug -DCMAKE_BUILD_TYPE=Debug
call xcopy Data build-em\debug\Data\ /s /d /y
call cmake --build build-em\debug
if %ERRORLEVEL% GEQ 1 goto done
call cmd /k npx http-server build-em\debug
echo --------------------------------------------------------------------------
echo ----------------------- Finished Building Debug ------------------------
echo --------------------------------------------------------------------------

:done
pause
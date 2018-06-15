set TOOLCHAIN="-DCMAKE_TOOLCHAIN_FILE=../../cmake/msvc.cmake"
set GENERATOR=Visual Studio 14 2015

if exist x32win rd x32win /S/Q
mkdir x32win
cd x32win

cmake ../../generate -G "%GENERATOR%" %TOOLCHAIN% -T v140_xp -DPASSOA_MAIN=ON -DPASSOA_MAKE_JS=ON -DCMAKE_INSTALL_PREFIX=./pack
rem cmake --build ./ --target install
cd ..
pause
set TOOLCHAIN="-DCMAKE_TOOLCHAIN_FILE=../../cmake/msvc.cmake"
set GENERATOR=Visual Studio 14 2015

if not exist 3rd mkdir 3rd
cd 3rd
cmake ../../3rd/portaudio -G "%GENERATOR%" -T v140_xp -DPA_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=../../stage
cmake --build ./ --target install

cd ..
if not exist x32win mkdir x32win
cd x32win

cmake ../../generate -G "%GENERATOR%" %TOOLCHAIN% -T v140_xp -DPASSOA_MAIN=ON -DPASSOA_MAKE_JS=ON -DCMAKE_INSTALL_PREFIX=../../output
rem cmake --build ./ --target install
cd ..
pause
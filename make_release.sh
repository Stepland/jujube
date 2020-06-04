rm -rf build
mkdir build
meson build
cd build
ninja
cp ../assets assets -r
cp -t . /mingw64/bin/libbrotlidec.dll /mingw64/bin/libharfbuzz-0.dll /mingw64/bin/libpng16-16.dll /mingw64/bin/libvorbis-0.dll /mingw64/bin/zlib1.dll /mingw64/bin/libbz2-1.dll /mingw64/bin/libiconv-2.dll /mingw64/bin/libsfml-audio-2.dll /mingw64/bin/libvorbisenc-2.dll /mingw64/bin/libfreetype-6.dll /mingw64/bin/libintl-8.dll /mingw64/bin/libsfml-graphics-2.dll /mingw64/bin/libvorbisfile-3.dll /mingw64/bin/libgcc_s_seh-1.dll /mingw64/bin/libogg-0.dll /mingw64/bin/libsfml-system-2.dll /mingw64/bin/libwinpthread-1.dll /mingw64/bin/libFLAC-8.dll /mingw64/bin/libglib-2.0-0.dll /mingw64/bin/libopenal-1.dll /mingw64/bin/libsfml-window-2.dll /mingw64/bin/libbrotlicommon.dll /mingw64/bin/libgraphite2.dll /mingw64/bin/libpcre-1.dll /mingw64/bin/libstdc++-6.dll

rm -rf jujube@exe meson* build.ninja .ninja* compile_commands.json test

cd ..
zip jujube.zip -r build
mv -t ../ jujube.zip

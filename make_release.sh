rm -rf build
mkdir build
meson build
meson configure build -Ddebug=false -Doptimization=2 
cd build
ninja
cp ../assets assets -r
../utils/copy_dependencies.py -f jujube.exe

rm -rf jujube@exe meson* build.ninja .ninja* compile_commands.json test

cd ..
zip jujube.zip -r build
mv -t build jujube.zip

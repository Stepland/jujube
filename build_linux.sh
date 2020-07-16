rm -rf build
mkdir build
meson build
meson configure build -Ddebug=false -Doptimization=2
cd build
ninja
cp ../assets assets -r

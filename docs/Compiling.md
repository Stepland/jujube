# Compiling jujube

jujube uses the meson build system.
Only time will tell if this turned out to be a wise choice, I wanted to try something other than CMake.

Regardless of the target platform the build steps are pretty much the same :

0. Get an up-to-date C++ compiler (compatible with C++17 at least)
0. Install (or build) the dependencies :
    - **meson** : the build system, installation explained [here](https://mesonbuild.com/Quick-guide.html)
    - **SFML 2.5.1** : the game framework
    - *possibly others*
0. Download the code from the repo
0. Go to your terminal and type
    ```bash
    $ cd {jujube repo root}
    $ CXX={your compiler} meson build
    $ cd build
    $ ninja
    ```
    (Replacing the `{...}`s with what you chose)
0. Copy the resulting `jujube` executable from the build folder to the project root folder.

and voila !

If you managed to compile jujube with another toolchain I did not mention here you're very welcome to do a PR appending the relevant parts of this doc.

## Linux

You need at least gcc `9.0.0`

The meson install is pretty straightforward.

For SFML, If you want to use another compiler than the one shipped with your system you may need to re-build SFML yourself using the compiler you chose. [Here's](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php) their docs. Don't forget to manually specify the compiler (and maybe the library install path) at the configuration step (when you run `meson build`).

## Windows

This section explains how to setup MSYS2 to compile jujube. MSYS2 is most likely *not* the only way to go but it's my favorite way of getting a very recent gnu-friendly toolchain on windows.

### MSYS2

Installing MSYS2 is extremly straightforward and detailed [on their website](https://www.msys2.org/).
Once you're done `pacman -Syu`ing and `pacman -Su`ing your system you can install the required packages for jujube :

```bash
$ pacman -S meson mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-sfml mingw-w64-x86_64-cairo
```

then you can just use the same build commands as the Linux section but from the `MSYS2 MinGW 64-bit` terminal

## MacOS
### clang-9 from brew's llvm package
If you do not have it already, install [Homebrew](https://brew.sh/).
When it's done use it to install basically everything :

```bash
$ brew install llvm sfml openal-soft
```

Once it's done (llvm is huge, go do something else while it installs), open a terminal, `cd` to your local copy of jujube's repo and use this carefully crafted command to setup your build folder :

```bash
$ LLVM=$(brew --prefix llvm) \
CC=$LLVM/bin/clang \
CXX=$LLVM/bin/clang++ \
LD=$LLVM/bin/ld.lld \
AR=$LLVM/bin/llvm-ar \
RANLIB=$LLVM/bin/llvm-ranlib \
PKG_CONFIG_PATH+=$(brew --prefix sfml)/lib/pkgconfig:$(brew --prefix openal-soft)/lib/pkgconfig \
LDFLAGS+="-L$LLVM/lib -Wl,-rpath,$LLVM/lib" \
CPPFLAGS+="-I$LLVM/include -I$LLVM/include/c++/v1/" \
meson build
```

then just

```bash
$ cd build
$ ninja
```

# Compiling jujube

jujube uses the meson build system.
Only time will tell if this turned out to be a wise choice, I wanted to try something other than CMake.

Regardless of the target platform the build steps are pretty much the same :

0. Get an up-to-date C++ compiler (compatible with C++17 at least, with support for `std::filesystem`)
0. Install (or build) the dependencies :
    - **meson** : the build system, installation explained [here](https://mesonbuild.com/Quick-guide.html)
    - **SFML 2.5.1** : the game framework
0. Download the code from the repo
0. Go to your terminal and type
    ```bash
    $ cd {jujube repo root}
    $ CXX={your compiler} meson build
    $ cd build
    $ ninja
    ```
    (Replacing the `{...}`s with what you chose)

and voila !

## Linux

For the compiler, `gcc 8.0.0` and up should work but I only tested with `gcc 9.2.1`

The meson install is pretty straightforward.

For SFML, If you want to use another compiler than the one shipped with your system you may need to re-build SFML yourself using the compiler you chose. [Here's](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php) their docs. Don't forget to manually specify the compiler at the configuration step.

## Windows

This section explains how to setup MSYS2 to compile jujube. MSYS2 is most likely *not* the only way to go but it's my favorite way of getting a very recent c++-friendly toolchain on windows.

If you managed to compile jujube with another toolchain you're very welcome to do a PR appending this part of the documentation.

### MSYS2

Installing MSYS2 is extremly straightforward and detailed [on their website](https://www.msys2.org/).
Once you're done `pacman -Syu`ing and `pacman -Su`ing your system you can install the required packages for jujube :

```bash
$ pacman -S mingw-w64-x86_64-sfml
```

then you can just use the same build commands as Linux
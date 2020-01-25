# Compiling jujube

jujube uses the meson build system.
Only time will tell if this turned out to be a wise choice, I just can't stand CMake.

jujube needs a *very* up to date C++ compiler for I'm using stuff like `std::filesystem` which is, at the time of writing, easily availble only on the very last version of your favorite compiler

## Linux

### Dependencies

- **SFML 2.5.1** : You will most likely need to build SFML yourself, and with the same compiler as the one you will use for jujube because if you just get precompiled binaries for SFML from your package manager you *will* have compatibility problems. [Here's](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php) their docs.
- **meson** : Installation explained [here](https://mesonbuild.com/Quick-guide.html)

### Building

let's say you downloaded the source code in `jujube` and you decided to compile with `gcc-9`

```bash
$ cd jujube
$ CXX=gcc-9 meson build
$ cd build
$ ninja
```
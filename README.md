# realpaver

## How to build

Realpaver is using meson build system (https://github.com/mesonbuild/meson).

In Realpaver root folder run:

```bash
meson setup build
```

The folder build will be generated (you can change this name to a preferred one), then everything is done in this folder:

```bash
cd build
ninja
```

## How to change building options

Theses options can be used directly with the `meson setup` command, but also afterwards.
In realpaver root folder, run `meson configure` command, while specifying one of the supported options:

### Some of tge pre-defined options:

- `--prefix PATH`: specify the path where to install (default is /usr/local)
- `--backend BACKEND`: specify the backend used to build (default is ninja, can be replace by vs,vs2010,vs2012,vs2013,vs2015,vs2017,vs2019,vs2022,xcode,none)
- `--buildtype BTYPE`: specify the build type to use (default: debug, can be replaced by plain,debug,debugoptimized,release,minsize,custom)
- `--optimization OPT`: specify the optimization level (default: 0, can be replaced by plain,0,g,1,2,3,s)

### Realpaver specific options:

- `-DLP_LIB=LIBNAME`: specify the linear programing library/solver to use (default: Clp, can be replaced by Highs, Soplex)
- `-DINTERVAL_lib=LIBNAME`: specify the interval library to use (default: Gaol)
- `-DASSERT=BOOLVAL`: activate assertions (default: true)
- `-DDEBUG=BOOLVAL`: activate debug messages (default: false)
- `-DLOG=BOOLVAL`: activate logging (default: false)

Example of meson setup command:

```bash
meson setup build --prefix /opt/homebrew/Cellar/realpaver/1.0.0 -DLP_LIB=Highs -DLOG=true
```

### How to compile with Gurobi?

**Requirements:** pkg-config

1. Modify the gurobi100.pc file in the realpaver folder with your own configuration for gurobi
2. Copy this file to a folder where meson will look at when searching for dependencies (i.e. /opt/homebrew/Cellar/yaml-cpp/0.7.0/share/pkgconfig/ in macOS with homebrew)
3. Configure your build folder with gurobi as LP solver:

```bash
meson setup build -DLP_LIB=Gurobi
```

## How to run tests

In the build folder run:

```bash
ninja test
```

## How to install

In the build folder run:

```bash
ninja install
```

## More documentation on meson:

- https://mesonbuild.com/index.html
- https://mesonbuild.com/Running-Meson.html
- https://mesonbuild.com/Commands.html
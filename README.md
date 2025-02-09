# RealPaver

The RealPaver library provides a C++ API to deal with interval analysis, constraint programming and global optimization techniques.

## Documentation

[https://realpavergroup.univ-nantes.io/realpaver/](https://realpavergroup.univ-nantes.io/realpaver/)

## Pre-requisite

### Mandatory dependencies

- meson build system (https://mesonbuild.com/)
- a supported interval library (eg: GAOL https://github.com/goualard-f/GAOL)
- a supported linear solver library:
  - CLP: https://github.com/coin-or/Clp
  - HiGHS: https://highs.dev/
  - SoPlex: https://soplex.zib.de/
  - Gurobi: https://www.gurobi.com/solutions/gurobi-optimizer/

### Optional dependencies

- lex, yacc (or flex, bison)
- doxygen

Be careful that your system uses recent versions of these libraries or tools!

## How to build

Realpaver is using meson build system for compilation, test and installation.

In Realpaver root folder run:

```bash
meson setup build
```

The folder build will be generated (you can change this name to a preferred one), then everything is done in this folder:

```bash
cd build
meson compile
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
- `-DINTERVAL_LIB=LIBNAME`: specify the interval library to use (default: Gaol)
- `-DLOCAL_SOLVER=LIBNAME`: specify the local optimization solver to use (default: none, can be replaced by NLOPT, IPOPT)
- `-DASSERT=BOOLVAL`: activate assertions (default: true)
- `-DLOG=BOOLVAL`: activate logging (default: false)

Example of meson setup command:

```bash
meson setup build --prefix /opt/homebrew/Cellar/realpaver/1.0.0 -DLP_LIB=Highs -DLOG=true
```

### How to compile with Gurobi ?

**Requirements:** pkg-config

1. Modify the gurobi.pc file in the realpaver folder with your own configuration for gurobi
2. Copy this file to a folder where meson will look at when searching for dependencies (e.g /opt/homebrew/share/pkgconfig in macOS with homebrew) or add its path to the environment variable PKG_CONFIG_PATH
3. Configure your build folder with gurobi as LP solver:

```bash
meson setup build -DLP_LIB=Gurobi
```

## How to run tests

In the build folder run:

```bash
meson test
```

## How to install

In the build folder run:

```bash
meson install
```

## More documentation on meson:

- https://mesonbuild.com/index.html
- https://mesonbuild.com/Running-Meson.html
- https://mesonbuild.com/Commands.html

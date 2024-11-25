# How to install

No binary distribution is provided, so it must be compiled from source code.

RealPaver installation process is based on mesonbuild system (https://mesonbuild.com/).

## Requirements

Required software or libraries:

- mesonbuild, then a minimal python environment
- a C/C++ compiler: gcc, clang or msvc
- an interval library: [Gaol](https://github.com/goualard-f/GAOL) or inum64
- a linear programming library: [Clp](https://github.com/coin-or/Clp), [Highs](https://highs.dev/), [Soplex](https://soplex.zib.de/) or [Gurobi](https://www.gurobi.com/downloads/gurobi-software/)

Optional software or libraries:

- doxygen
- mkdocs
  - mkdocs-material
  - mkdoxy
- lex (or flex) and yacc (or bison)
- pkg-config


## Getting the source code

It requires to compile from source code.

You can get the code using git:

```bash
git clone https://gitlab.univ-nantes.fr/realpavergroup/realpaver
```

Or you can download the lastest stable: [zip](https://gitlab.univ-nantes.fr/realpavergroup/realpaver/-/archive/main/realpaver-main.zip)

## Configure and compile

With mesonbuild, the defaults steps are:

1. Configure the build folder:

```bash
meson setup build
```

2. Compile

```bash
cd build
meson compile
```

3. Install

```bash
meson install
```

### RealPaver options

Specific RealPaver options can be used:

- `-DLP_LIB=LIBNAME`: specify the linear programming library/solver to use (default: Clp, can be replaced by Highs, Soplex or Gurobi).
- `-DINTERVAL_LIB=LIBNAME`: specify the interval library to use (default: Gaol, can be replaced by inum64)
- `-DASSERT=BOOLVAL`: activate assertions (default: false)
- `-DDEBUG=BOOLVAL`: activate debug messages (default: false)
- `-DLOG=BOOLVAL`: activate logging (default: false)


### Generic mesonbuild options:

- `--prefix=PATH`: specify the path where to install (the default one on most UNIX systems is /usr/local)
- `--buildtype BTYPE`: specify the build type to use (default: debug, can be replaced by plain,debug,debugoptimized,release,minsize,custom)

More on the official mesonbuild documentation: [https://mesonbuild.com/Builtin-options.html](https://mesonbuild.com/Builtin-options.html)

### Example of configuration

For macos, if using [Homebrew](https://brew.sh/), here is an example where the prefix target the homebrew cellar:

```bash
meson setup build --prefix=/opt/homebrew/Cellar/realpaver/1.0.0 -DLP_LIB=clp -DINTERVAL_LIB=gaol
```


## Test

Before installing, it is better to check all tests passed (to be run in the build folder):

```bash
meson test
```

## Generate documentation

Go to the **doc** directory.

### Build documentation

Run *mkdocs*:

```bash
mkdocs build
```

The available site is in **doc/site** folder.

### View documentation

Run *mkdocs*:

```bash
mkdocs serve
```

You have to open your default web browser to [http://127.0.0.1:8000/realpaver/](http://127.0.0.1:8000/realpaver/).

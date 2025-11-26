# Containers for realpaver

Several containers can be generated.

## Common features:

- Ubuntu base (latest version from DockerHub)
- All common dependencies for realpaver: g++, git, pkg-config, cmake, meson, ninja-build, bison, flex, zlib
- All common dependencies to build the docker image: wget, unzip

## Different features:

- Dockerfile.clp.inum64: configure realpaver with CLP linear solver and inum64 interval library (**recommended one**)
- Dockerfile.highs.inum64: configure realpaver with HiGHS linear solver and inum64 interval library
- Dockerfile.clp.goal: configure realpaver with CLP linear solver and GAOL interval library
- Dockerfile.highs.goal: configure realpaver with HiGHS linear solver and GAOL interval library


## How to build images

```bash
docker build . -f Dockerfile.highs.inum64 -t realpaver/highs-gaol
```

### Possible build arguments

- assert: **true**/false, activate assertions in RealPaver
- log: **true**/false, activate logging in RealPaver
- branch: **main**/joss, select branch from GitHub repositroy
- test: true/**false**, run `meson test` after compiling library

Example of use:

```bash
docker build . -f Dockerfile.highs.inum64 -t realpaver/highs-inum64 --build-arg branch=joss --build-arg test=true
```


## How to run an image

```bash
docker run -it realpaver/highs-inum64 /bin/bash
```

Using argument `-it` gives you access the command line interface inside the container. Then, you can run anything available in it like:

```bash
cd realpaver/build
meson test
```

or


```bash
cd realpaver
./build/bin/rp_solver ./benchmarks/csp/Apollonius.rp
```

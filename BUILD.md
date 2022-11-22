## How to build

At the time of writing, GCC-AVR 11 seems to be the optimal version to
use as it produces the smallest (working) firmware.

The general process is as follows:

* install all required dependencies: gcc-avr, avr-libc, cmake, make and python3.
* compile the code
* use one of the products for flashing

Compilation in a shell works like this:

```sh
mkdir -p build && cd build
cmake ..
make
```

`p600firmware.bin` and `p600firmware.syx` are the most relevant build
products for flashing via USB or MIDI, respectively. These files are
created by default. Try `make help` for a list of all available build
targets.

### Using docker or podman

The most flexible approach is to use a compiler toolchain inside a
dockerized Linux environment as this can easily build and set up on
all operating systems.

It is also possible to specify a certain compiler version and/or have
multiple versions readily available.

The Dockerfile supplied with the repository can be used to create a
Docker image based on Alpine Linux and provides gcc-avr, make and python3.

An overview of the several gcc-avr versions for several Alpine releases can be found [here](https://pkgs.alpinelinux.org/packages?name=gcc-avr&branch=&repo=&arch=&maintainer=).

```sh
# Build gcc 12 using Alpine Linux edge (default)
docker build -t avr-toolchain:12 .

# Build gcc 11 using Alpine Linux 3.16
docker build --build-arg ALPINE_VERSION=3.16 -t avr-toolchain:11 .

# Build gcc 10 using Alpine Linux 3.15
docker build --build-arg ALPINE_VERSION=3.15 -t avr-toolchain:10 . 

# Compile project
docker run --rm -v $(pwd):/build avr-toolchain:11 \
  sh -c "mkdir -p build && cd build && cmake .. && make"
```

### Building on MacOS

On MacOS, first set up a [Homebrew](https://brew.sh) environment. 

By adding the
[homebrew-avr](https://github.com/osx-cross/homebrew-avr/) tap from
here, several GCC compiler versions can be installed.

`avr-gcc@10` and `avr-gcc@11` have been tested successfully on a M1
Macbook Air and should work on an Intel Macbook as well.

### Building on Linux

Depending on the used distribution, it can be hard to find a working
pre-packaged compiler toolchain.

On Debian for example, there is only GCC-AVR version 5 available by
default.

Fedora on the other hand offers modern GCC versions in their more
recent [releases](https://src.fedoraproject.org/rpms/avr-gcc).

### Building on Windows

Work in progress.

# Dronecrypt on 64-bit processors

These codes are based on FourQ library (https://github.com/Microsoft/FourQlib).

Various public key optimization techniques are harnessed to offer a light-weight framework for Internet of Drones. Dronecrypt.c Dronecrypt.h files include key exchange, digital signature and public key encryption algorithms to offer a full framework.

## Supported platforms

This implementation is supported in a wide range of platforms including x64, x86, 32-bit ARM and 64-bit ARM,
running Windows or Linux. We have tested the library with Microsoft Visual Studio 2015, GNU GCC v4.9 and 
clang v3.8. 

See instructions below to choose an implementation option and compile on one of the supported platforms. 

## Implementation options

The following compilation options are available for the `FourQ_64bit_and_portable` implementation:

* A portable implementation (enabled by the "GENERIC" option).
* Optimized implementations for x64 and 64-bit ARM (ARMv8). Note that the rest of platforms are only supported
  by the generic implementation. 
* Use of AVX or AVX2 instructions enabled by defining `_AVX_` or `_AVX2_` (Windows) or by the "AVX" and "AVX2" 
  options (Linux).
* Optimized x64 assembly implementations in Linux.
* Use of fast endomorphisms enabled by the "USE_ENDO" option.

Follow the instructions below to configure these different options.

## Instructions for Linux

### Building the library and executing the tests with GNU GCC or clang

To compile on Linux using the GNU GCC compiler or the clang compiler, execute the following command from the 
command prompt:

```sh
$ make ARCH=[x64/x86/ARM/ARM64] CC=[gcc/clang] ASM=[TRUE/FALSE] AVX=[TRUE/FALSE] AVX2=[TRUE/FALSE] 
     EXTENDED_SET=[TRUE/FALSE] USE_ENDO=[TRUE/FALSE] GENERIC=[TRUE/FALSE] SERIAL_PUSH=[TRUE/FALSE] 
```

After compilation, run `dronecrypt_tests`.

By default GNU GCC is used, as well as the endomorphisms and the extended settings.

In the case of x64, AVX2 instructions and the high-speed assembly implementation are enabled by default.
In the case of x86 and ARM, the portable ("GENERIC") implementation is used by default.

For example, to compile the optimized x64 implementation in assembly with GNU GCC using the efficient
endomorphisms on a machine with AVX2 support (e.g, Intel's Haswell or Broadwell), execute:

```sh
$ make ARCH=x64
```

For example, to compile the optimized ARM64 implementation with GNU GCC using the efficient endomorphisms, 
execute:

```sh
$ make ARCH=ARM64
```

As another example, to compile the portable implementation with clang using the efficient endomorphisms 
on an x86 machine, execute:

```sh
$ make ARCH=x86 CC=clang
```

`SERIAL_PUSH` can be enabled in some platforms (e.g., AMD without AVX2 support) to boost performance.

By default `EXTENDED_SET` is enabled, which sets the following compilation flags: `-fwrapv -fomit-frame-pointer 
-march=native`. To disable this, use `EXTENDED_SET=FALSE`.
Users are encouraged to experiment with the different flag options.

Whenever an unsupported configuration is applied, the following message will be displayed: `#error -- "Unsupported configuration". 
For example, the use of assembly or any of the AVX options is not supported when selecting the portable implementation 
(i.e., if `GENERIC=TRUE` or if `ARCH=[x86/ARM]`). 

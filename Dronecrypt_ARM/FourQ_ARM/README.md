# Dronecrypt on 32-bit ARM

These codes are based on FourQ library (https://github.com/Microsoft/FourQlib).

Various public key optimization techniques are harnessed to offer a light-weight framework for Internet of Drones. Dronecrypt.c Dronecrypt.h files include key exchange, digital signature and public key encryption algorithms to offer a full framework.

## Supported platforms

This implementation is supported on ARM platforms and includes two variants: 

*  Implementation for ARM processors based on ARMv6 and ARMv7 architectures. This implementation was optimized
     for a first generation Raspberry Pi using a 700 MHz ARM1176JZF-S processor (ARMv6 architecture).
* Implementation for ARM Cortex-M4 processors based on the ARMv7-M architecture. This implementation was 
     developed and optimized on a STM32F4Discovery development board containing a Cortex-M4 STM32F407VG microcontroller (ARMv7-M architecture). It should be possible to extend the support to Cortex-M3 and Cortex-M7 based devices with small modifications.   

See instructions below to choose an implementation option and compile on one of the supported platforms.

## Instructions

### Building the library for ARMv6 or ARMv7

To compile on Linux using the GNU GCC compiler or the clang compiler, execute the following command from the 
command prompt:
 
```sh 
$ make CC=[gcc/clang] USE_ENDO=[TRUE/FALSE] EXTENDED_SET=[TRUE/FALSE] CACHE_MEM=[TRUE/FALSE]
```

After compilation, run `dronecrypt_tests`.

By default GNU GCC is used, as well as endomorphisms and extended settings. Similarly, `CACHE_MEM=TRUE` is set
by default indicating that the targeted platform contains a cache memory.

For example, to compile using GNU GCC with the efficient endomorphisms, execute:

```sh 
$ make
```

As another example, to compile using clang with the efficient endomorphisms, execute:

```sh 
$ make CC=clang
```

By default `EXTENDED_SET` is enabled, which sets the following compilation flags: `-fwrapv -fomit-frame-pointer 
-funroll-loops`. To disable this, use `EXTENDED_SET=FALSE`.
Users are encouraged to experiment with the different flag options.

Note: The code is not tested ARMv6 or ARMv7 architecture

### Building the library for Cortex-M4 on the STM32F4DISCOVERY board

The following instructions have been tested on a Ubuntu 16.04 Linux machine.

First, install the ARM GNU GCC cross-compiler on the server machine:

```sh 
$ sudo apt-get install gcc-arm-none-eabi libc6-dev-i386
```

Then, download, build and install [stlink](https://github.com/texane/stlink):

```sh 
$ sudo apt-get install libusb-1.0-0-dev
$ git clone https://github.com/texane/stlink.git
$ cd stlink
$ make
$ cd build/Release/ && sudo make install
```

To compile the code, execute the following command from the `FourQ_ARM` folder on the server machine:

```sh 
$ make -f makefile_Cortex-M4 USE_ENDO=[TRUE/FALSE]
```

Power the STM32F4DISCOVERY board (with a USB to mini-USB cable) and connect it to the server machine via a 
USB-TTL converter as follows:

```sh 
VDD -> VDD
GND -> GND 
TX  -> PA3 
RX  -> PA2 
```

Then, run from the server machine:

```sh 
$ sudo ./tests_Cortex-M4/monitor.sh
```

From a different terminal window on the server machine, program the device with one of the following commands
from the `FourQ_ARM` folder:

```sh 
$ st-flash write tests_Cortex-M4/dronecrypt_tests.bin 0x8000000
```

The tests should begin to run on the first terminal window.

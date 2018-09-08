# Dronecrypt on 8-bit AVR

These codes are based on microFourQ library (https://github.com/geovandro/microFourQ-AVR). 

Various public key optimization techniques are harnessed to offer a light-weight framework for Internet of Drones. Dronecrypt.c Dronecrypt.h files include key exchange, digital signature and public key encryption algorithms to offer a full framework.

### IMPORTANT SECURITY NOTE

Random values are generated with `rand()`. This is NOT a cryptographically secure function.
Users should replace this function with a cryptographically-secure PRNG (see [`random.c`](src/random/random.c)) .

## Instructions

Download the IAR Workbench for AVR (https://www.iar.com/iar-embedded-workbench/).

Open the project file [`microFourQ_AVR.eww`](iar-ide/microFourQ_AVR.eww) and click on `Project > Rebuild All`.

Project settings can be accessed and modified by going to `Project > Options...`. 

Note: To run these files on ATmega2560, cfgm2560.xcl file should be updated such that: -D_..X_SRAM_BASE=5000 and -D_..X_SRAM_END=6FFF. Otherwise, from the options you can change the target processor to ATxmega256A3.

## License
   
These codes and microFourQ are licensed under the MIT License; see [`License_microFourQ.txt`](License_microFourQ.txt) for details.

It is based on the Microsoft library **FourQlib** (https://github.com/Microsoft/FourQlib), which is also licensed under MIT.

The BLAKE2b implementation, written by Thomas Pornin, is under an MIT-like open source license (see [`blake.c`](src/blake2b/blake.c)).

The files [`mp_add.s`](src/AVR/mp_add.s) and [`mp_sub.s`](src/AVR/mp_sub.s), written by Johann Groszschaedl and copyrighted by University of Luxembourg, are licensed under GPLv3. 
 

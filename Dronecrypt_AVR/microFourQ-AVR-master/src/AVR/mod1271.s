//////////////////////////////////////////////////////////////////////////////
// Fast Reduction: r = c mod p; Incomplete arithmetic [0, 2^128-1]          //
//                                                                          //
// Authors: Zhe Liu, Geovandro Pereira, Hwajeong Seo                        //
//////////////////////////////////////////////////////////////////////////////

NAME mod1271            ; module name
PUBLIC mod1271          ; make the main label vissible outside this module
RSEG CODE               ; place program in 'CODE' segment

#define B0 R8
#define B1 R9
#define B2 R10
#define B3 R11
#define B4 R12
#define B5 R13
#define B6 R14
#define B7 R15
#define B8 R16
#define B9 R17
#define B10 R18
#define B11 R19
#define B12 R20
#define B13 R21
#define B14 R22
#define B15 R23
#define T0 R24
#define T1 R25
#define T2 R26
#define ZERO R27

mod1271:
;  R4–R15 and R24–R29
  PUSH R8
  PUSH R9
  PUSH R10
  PUSH R11
  PUSH R12
  PUSH R13
  PUSH R14
  PUSH R15
  
  PUSH R24
  PUSH R25
  PUSH R26
  PUSH R27
  PUSH R28
  PUSH R29

  MOVW R30, R16 // Product C
  
  CLR ZERO

  LDD B0, Z+0
  LDD B1, Z+1
  LDD B2, Z+2
  LDD B3, Z+3
  
  LDD B4, Z+4
  LDD B5, Z+5
  LDD B6, Z+6
  LDD B7, Z+7
  
  LDD B8, Z+8
  LDD B9, Z+9
  LDD B10, Z+10
  LDD B11, Z+11
  
  LDD B12, Z+12
  LDD B13, Z+13
  LDD B14, Z+14
  LDD B15, Z+15
  
  MOV T0, B15

  ANDI B15, 0x7f

  LSL T0
  ADC B0, ZERO
  ADC B1, ZERO
  ADC B2, ZERO
  ADC B3, ZERO

  ADC B4, ZERO
  ADC B5, ZERO
  ADC B6, ZERO
  ADC B7, ZERO

  ADC B8, ZERO
  ADC B9, ZERO
  ADC B10, ZERO
  ADC B11, ZERO

  ADC B12, ZERO
  ADC B13, ZERO
  ADC B14, ZERO
  ADC B15, ZERO
  
  MOV T0, B15
  CLR T1
  ADD T0, T0
  ADC T1, T1

  ANDI B15, 0x7f

  ADD B0, T1
  ADC B1, ZERO
  ADC B2, ZERO
  ADC B3, ZERO

  ADC B4, ZERO
  ADC B5, ZERO
  ADC B6, ZERO
  ADC B7, ZERO

  ADC B8, ZERO
  ADC B9, ZERO
  ADC B10, ZERO
  ADC B11, ZERO

  ADC B12, ZERO
  ADC B13, ZERO
  ADC B14, ZERO
  ADC B15, ZERO
  
  
  ST Z+, B0
  ST Z+, B1
  ST Z+, B2
  ST Z+, B3

  ST Z+, B4
  ST Z+, B5
  ST Z+, B6
  ST Z+, B7

  ST Z+, B8
  ST Z+, B9
  ST Z+, B10
  ST Z+, B11

  ST Z+, B12
  ST Z+, B13
  ST Z+, B14
  ST Z+, B15

  
  ;  R4–R15 and R24–R29
  POP R29
  POP R28
  POP R27
  POP R26
  POP R25
  POP R24 
  
  POP R15
  POP R14
  POP R13
  POP R12
  POP R11
  POP R10
  POP R9
  POP R8

  
  RET
END

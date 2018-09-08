//////////////////////////////////////////////////////////////////////////////
// Field Addition: r = a + b mod p; Incomplete arithmetic [0, 2^128-1]      //
//                                                                          //
// Authors: Zhe Liu, Hwajeong Seo                                           //
//////////////////////////////////////////////////////////////////////////////

NAME fpadd1271            ; module name
PUBLIC fpadd1271          ; make the main label vissible outside this module
RSEG CODE                 ; place program in 'CODE' segment

;  R4–R15 and R24–R29
; operand b: r21:r20
; operand a: r23:r22
; operand r: r25:r24

#define B R0
#define ZERO R0

#define A0 R1
#define A1 R2
#define A2 R3
#define A3 R11
#define A4 R12
#define A5 R13
#define A6 R14
#define A7 R15
#define A8 R16
#define A9 R17
#define A10 R18
#define A11 R19
#define A12 R20
#define A13 R21
#define A14 R22
#define A15 R23

fpadd1271:

  PUSH R11
  PUSH R12
  PUSH R13
  PUSH R14
  PUSH R15
 
  PUSH R26
  PUSH R27
  
  PUSH R28
  PUSH R29

  MOVW R30, R20 // R
  MOVW R26, R16 // A
  MOVW R28, R18 // B

  //Load A: 16 bytes
  LD A0, X+
  LD A1, X+
  LD A2, X+
  LD A3, X+

  LD A4, X+
  LD A5, X+
  LD A6, X+
  LD A7, X+

  LD A8, X+
  LD A9, X+
  LD A10, X+
  LD A11, X+

  LD A12, X+
  LD A13, X+
  LD A14, X+
  LD A15, X+

  // Add A and B
  LD B, Y+
  ADD A0, B
  LD B, Y+
  ADC A1, B
  LD B, Y+
  ADC A2, B
  LD B, Y+
  ADC A3, B

  LD B, Y+
  ADC A4, B
  LD B, Y+
  ADC A5, B
  LD B, Y+
  ADC A6, B
  LD B, Y+
  ADC A7, B

  LD B, Y+
  ADC A8, B
  LD B, Y+
  ADC A9, B
  LD B, Y+
  ADC A10, B
  LD B, Y+
  ADC A11, B

  LD B, Y+
  ADC A12, B
  LD B, Y+
  ADC A13, B
  LD B, Y+
  ADC A14, B
  LD B, Y+
  ADC A15, B


  // Get the part bigger than 2^127
  MOV B, A15
  ANDI A15, 0X7F
  ADD B, B
  CLR ZERO

  // Reduction: 2^127=1, thus, B16 is in [0, 3]
  ADC A0, ZERO
  ADC A1, ZERO
  ADC A2, ZERO
  ADC A3, ZERO

  ADC A4, ZERO
  ADC A5, ZERO
  ADC A6, ZERO
  ADC A7, ZERO

  ADC A8, ZERO
  ADC A9, ZERO
  ADC A10, ZERO
  ADC A11, ZERO

  ADC A12, ZERO
  ADC A13, ZERO
  ADC A14, ZERO
  ADC A15, ZERO

  //Store result
  ST Z+, A0
  ST Z+, A1
  ST Z+, A2
  ST Z+, A3

  ST Z+, A4
  ST Z+, A5
  ST Z+, A6
  ST Z+, A7

  ST Z+, A8
  ST Z+, A9
  ST Z+, A10
  ST Z+, A11

  ST Z+, A12
  ST Z+, A13
  ST Z+, A14
  ST Z+, A15
  
  POP R29
  POP R28
  POP R27
  POP R26

  POP R15
  POP R14
  POP R13
  POP R12
  POP R11


  RET
END

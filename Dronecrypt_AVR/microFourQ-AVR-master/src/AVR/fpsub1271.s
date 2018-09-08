/////////////////////////////////////////////////////////////
// Field subtraction: r = a - b mod p, p = 2^127-1         //
//                                                         //
// Authors: Zhe Liu, Hwajeong Seo                          //
/////////////////////////////////////////////////////////////

NAME fpsub1271            ; module name
PUBLIC fpsub1271          ; make the main label vissible outside this module
RSEG CODE                 ; place program in 'CODE' segment

;  R4-R15 and R24-R29
; operand b: r21:r20
; operand a: r23:r22
; operand r: r25:r24

#define B R0
#define ZERO R1

#define A0 R2
#define A1 R3
#define A2 R11
#define A3 R12
#define A4 R13
#define A5 R14
#define A6 R15
#define A7 R16
#define A8 R17
#define A9 R18
#define A10 R19
#define A11 R20
#define A12 R21
#define A13 R22
#define A14 R23
#define A15 R24



fpsub1271:

  
  PUSH R11
  PUSH R12
  PUSH R13
  PUSH R14
  PUSH R15
  
  PUSH R24
  PUSH R26
  PUSH R27
  PUSH R28
  PUSH R29

  MOVW R30, R20 // R
  MOVW R26, R16 // A
  MOVW R28, R18 // B

  CLR ZERO

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

  // Sub: A - B
  LD B, Y+
  SUB A0, B
  LD B, Y+
  SBC A1, B
  LD B, Y+
  SBC A2, B
  LD B, Y+
  SBC A3, B

  LD B, Y+
  SBC A4, B
  LD B, Y+
  SBC A5, B
  LD B, Y+
  SBC A6, B
  LD B, Y+
  SBC A7, B

  LD B, Y+
  SBC A8, B
  LD B, Y+
  SBC A9, B
  LD B, Y+
  SBC A10, B
  LD B, Y+
  SBC A11, B

  LD B, Y+
  SBC A12, B
  LD B, Y+
  SBC A13, B
  LD B, Y+
  SBC A14, B
  LD B, Y+
  SBC A15, B

  MOV B, A15
  
  ANDI A15, 0X7F
  
  ADC B, B

  // Reduction: 2^127=1, 
  SBC A0, ZERO
  SBC A1, ZERO
  SBC A2, ZERO
  SBC A3, ZERO

  SBC A4, ZERO
  SBC A5, ZERO
  SBC A6, ZERO
  SBC A7, ZERO

  SBC A8, ZERO
  SBC A9, ZERO
  SBC A10, ZERO
  SBC A11, ZERO

  SBC A12, ZERO
  SBC A13, ZERO
  SBC A14, ZERO
  SBC A15, ZERO

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

  POP R24  
  
  POP R15
  POP R14
  POP R13
  POP R12
  POP R11
  
  RET
END

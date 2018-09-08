//////////////////////////////////////////////////////////////////////////////
// Field Addition: r = a + b mod p; Incomplete arithmetic [0, 2^128-1]       //
//                                                                          //
// Authors: Zhe Liu, Hwajeong Seo                                           //
//////////////////////////////////////////////////////////////////////////////

NAME fpneg1271           ; module name
PUBLIC fpneg1271         ; make the main label vissible outside this module
RSEG CODE                ; place program in 'CODE' segment

;  R4-R15 and R24-R29
; operand b: r21:r20
; operand a: r23:r22
; operand r: r25:r24

#define A0 R0
#define A1 R1
#define A2 R2
#define A3 R3
#define A4 R11
#define A5 R12
#define A6 R13
#define A7 R14
#define A8 R15
#define A9 R16
#define A10 R17
#define A11 R18
#define A12 R19
#define A13 R20
#define A14 R21
#define A15 R22
#define A16 R23
#define B R24

fpneg1271:

  
  PUSH R11
  PUSH R12
  PUSH R13
  PUSH R14
  PUSH R15
  
  PUSH R24

  MOVW R30, R16 // A

  LDI A15, 0XFF
  MOV A0, A15
  MOV A1, A15
  MOV A2, A15
  MOV A3, A15
  
  MOV A4, A15
  MOV A5, A15
  MOV A6, A15
  MOV A7, A15
  
  MOV A8, A15
  MOV A9, A15
  MOV A10, A15
  MOV A11, A15
  
  MOV A12, A15
  MOV A13, A15
  MOV A14, A15
  LDI A15, 0X7F
 
  LDD B, Z+0
  SUB A0, B
  LDD B, Z+1
  SBC A1, B
  LDD B, Z+2
  SBC A2, B
  LDD B, Z+3
  SBC A3, B

  LDD B, Z+4
  SBC A4, B
  LDD B, Z+5
  SBC A5, B
  LDD B, Z+6
  SBC A6, B
  LDD B, Z+7
  SBC A7, B

  LDD B, Z+8
  SBC A8, B
  LDD B, Z+9
  SBC A9, B
  LDD B, Z+10
  SBC A10, B
  LDD B, Z+11
  SBC A11, B

  LDD B, Z+12
  SUB A12, B
  LDD B, Z+13
  SBC A13, B
  LDD B, Z+14
  SBC A14, B
  LDD B, Z+15
  SBC A15, B
  
  
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

  POP R24
  
  POP R15
  POP R14
  POP R13
  POP R12
  POP R11
  
  RET
END
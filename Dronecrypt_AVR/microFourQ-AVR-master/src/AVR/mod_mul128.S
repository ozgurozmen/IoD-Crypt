////////////////////////////////////////////////////////////////////////////////////////////
// Multi-Precision modular multiplication: c = a * b mod 2^127-1                          // 
// Adapted from e-print paper: 2014/592                                                   //
//                                                                                        //
// Authors: Zhe Liu, Geovandro Pereira and Hwajeong Seo                                   //
////////////////////////////////////////////////////////////////////////////////////////////

NAME mod_mul128            ; module name
PUBLIC mod_mul128          ; make the main label vissible outside this module
RSEG CODE                   ; place program in 'CODE' segment

; operand a: r17:r16
; operand b: r19:r18
; operand r: r21:r20

#define B0 R20
#define B1 R21
#define B2 R22
#define B3 R23
#define B4 R24
#define B5 R25
#define B6 R8
#define B7 R9
#define B8 R4
#define B9 R5
#define B10 R6
#define B11 R7
#define B12 R10
#define B13 R11
#define B14 R12
#define B15 R13
#define T0 R26
#define T1 R27
#define ZERO R27


mod_mul128:
;  R4?R15 and R24?R29
  
  PUSH R4
  PUSH R5
  PUSH R6
  PUSH R7
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

  //SAVE R20 :: RESULT C

  IN R30, 0x3D
  IN R31, 0x3E
  SBIW R30, 34
  OUT 0x3D, R30
  OUT 0x3E, R31
  ADIW R30, 1

  PUSH R20
  PUSH R21  
  
 
  MOVW R28, R16 //Y = &a
  MOVW R26, R18 //X = &b

  ;------ level 1: compute L ------

  ; init registers with zero
  CLR R20
  CLR R21
  MOVW R16, R20

  ;--- level 2: compute L ---
  LD R2, X+   ;b0
  LD R3, X+   ;b1
  LD R4, X+   ;b2
  LD R5, X+   ;b3
  LDD R6, Y+0 ;a0
  LDD R7, Y+1 ;a1
  LDD R8, Y+2 ;a2
  LDD R9, Y+3 ;a3

  MUL R2, R8   ;a2*b0
  MOVW R12, R0 ;R13:R12 <- b0*a2
  MUL R2, R6   ;a0*b0
  MOVW R10, R0 ;R11:R10 <- b0*a0
  MUL R2, R7   ;b0*a1
  ADD R11, R0  ;R11 <- b0*a0[1] + b0*a1[0]
  ADC R12, R1  ;R12 <- b0*a2[0] + b0*a1[1]
  ADC R13, R21 ;R13 <- b0*a2[1] + c
  MUL R3, R9   ;b1*a3
  MOVW R14, R0 ;R15:R14 <- b1*a3

  MUL R2, R9   ;b0*a3
  MOVW R18, R0 ;R19:R18 <- b0*a3
  MUL R3, R6   ;b1*a0
  ADD R11, R0  ;b1*a0[0] + b0*a1[0] 
  ADC R12, R1  ;R12 <- b0*a2[0] + b0*a1[1] + b1*a0[1] + c
  ADC R13, R18
  ADC R19, R21
  MUL R3, R7   ;b1*a1
  ADD R12, R0  ;R12 <- (b0*a2[0] + b0*a1[1] + b1*a0[1]) + a1*b1[0]
  ADC R13, R1
  ADC R19, R21
  MUL R4, R9   ;b2*a3
  ADD R14, R19
  ADC R15, R0
  ADC R16, R1

  MUL R4, R8   ;a2*b2
  MOVW R18, R0
  MUL R4, R6   ;a0*b2
  ADD R12, R0  ;R12 <- (b0*a2[0] + b0*a1[1] + b1*a0[1] + a1*b1[0]) + a0*b2[0]
  ADC R13, R1
  ADC R14, R18 
  ADC R19, R21
  MUL R3, R8   ;b1*a2
  ADD R13, R0
  ADC R14, R1
  ADC R19, R21
  MUL R5, R9   ;b3*a3
  ADD R15, R19
  ADC R16, R0
  ADC R17, R1

  MUL R5, R7   ;b3*a1
  MOVW R18, R0
  MUL R4, R7   ;b2*a1
  ADD R13, R0
  ADC R18, R1
  ADC R19, R21
  MUL R5, R6   ;b3*a0
  ADD R13, R0
  ADC R18, R1
  ADC R19, R21
  MUL R5, R8   ;b3*a2
  ADD R14, R18
  ADC R0, R19
  ADC R1, R21
  ADD R15, R0
  ADC R16, R1
  ADC R17, R21
  STD Z+0, R10
  STD Z+1, R11
  STD Z+2, R12
  STD Z+3, R13
  
  ;--- load b4..b7 and a4..a7 ---
  MOVW R10, R20
  LD R18, X+
  LD R19, X+
  LD R20, X+
  ; R21 is loaded later
  LDD R22, Y+4
  LDD R23, Y+5
  LDD R24, Y+6
  LDD R25, Y+7

  ;--- level 2: compute H + (l3,l4,l5) ---
  MUL R18, R22 ; a4*b4
  ADD R14, R0
  ADC R15, R1
  ADC R16, R21
  ADC R11, R21  

  MUL R18, R23
  ADD R15, R0
  ADC R16, R1
  ADC R11, R21
  MUL R19, R22
  ADD R15, R0
  ADC R16, R1
  ADC R17, R11
  ADC R10, R21

  MUL R18, R24
  ADD R16, R0 
  ADC R17, R1
  ADC R10, R21
  MUL R19, R23
  ADD R16, R0
  ADC R17, R1
  ADC R10, R21
  MUL R20, R22
  ADD R16, R0
  ADC R17, R1
  ADC R10, R21

  CLR R11
  MUL R18, R25
  ADD R17, R0
  ADC R10, R1
  ADC R11, R21
  MUL R19, R24
  ADD R17, R0
  ADC R10, R1
  ADC R11, R21
  MUL R20, R23
  ADD R17, R0
  ADC R10, R1
  ADC R11, R21
  
  ;--- subtract a0-a4 ---
  SUB R2, R18
  SBC R3, R19
  SBC R4, R20
  ; load b7 to R18
  LD R18, X+
  SBC R5, R18
  ; 0xff if carry and 0x00 if no carry
  SBC R0, R0
  
  ;--- subtract b0-b4 ---
  SUB R6, R22
  SBC R7, R23
  SBC R8, R24
  SBC R9, R25
  ; 0xff if carry and 0x00 if no carry
  SBC R1, R1

  ;--- absolute values ---        
  EOR R2, R0
  EOR R3, R0
  EOR R4, R0
  EOR R5, R0  
  EOR R6, R1
  EOR R7, R1
  EOR R8, R1
  EOR R9, R1  
  NEG R0
  NEG R1
  ADD R2, R0
  ADC R3, R21
  ADC R4, R21
  ADC R5, R21  
  ADD R6, R1
  ADC R7, R21
  ADC R8, R21
  ADC R9, R21  
  EOR R0, R1
  BST R0, 0 
  
  ;--- continue ---
  MUL R18, R22
  ADD R17, R0
  ADC R10, R1
  ADC R11, R21

  MUL R19, R25
  CLR R19
  ADD R10, R0
  ADC R11, R1
  ADC R19, R21
  MUL R20, R24
  ADD R10, R0
  ADC R11, R1
  ADC R19, R21
  MUL R18, R23
  ADD R10, R0
  ADC R11, R1
  ADC R19, R21

  MUL R20, R25
  CLR R20
  ADD R11, R0
  ADC R19, R1
  ADC R20, R21
  MUL R18, R24
  ADD R11, R0
  ADC R19, R1
  ADC R20, R21

  MUL R18, R25
  ADD R19, R0
  ADC R20, R1

  ;--- level 2: compute M ---
  CLR R24
  CLR R25
  CLR R18

  MUL R2, R6
  MOVW R22, R0

  MUL R2, R7
  ADD R23, R0
  ADC R24, R1
  MUL R3, R6
  ADD R23, R0
  ADC R24, R1
  ADC R25, R21

  MUL R2, R8
  ADD R24, R0
  ADC R25, R1
  ADC R18, R21
  MUL R3, R7
  ADD R24, R0
  ADC R25, R1
  ADC R18, R21
  MUL R4, R6
  ADD R24, R0
  ADC R25, R1
  ADC R18, R21

  MUL R2, R9
  CLR R2
  ADD R25, R0
  ADC R18, R1
  ADC R2, R21
  MUL R3, R8
  ADD R25, R0
  ADC R18, R1
  ADC R2, R21
  MUL R4, R7
  ADD R25, R0
  ADC R18, R1
  ADC R2, R21
  MUL R5, R6
  ADD R25, R0
  ADC R18, R1
  ADC R2, R21

  MUL R3, R9
  CLR R3
  ADD R18, R0
  ADC R2, R1
  ADC R3, R21
  MUL R4, R8
  ADD R18, R0
  ADC R2, R1
  ADC R3, R21
  MUL R5, R7
  ADD R18, R0
  ADC R2, R1
  ADC R3, R21

  MUL R4, R9
  CLR R4
  ADD R2, R0
  ADC R3, R1
  ADC R4, R21
  MUL R5, R8
  ADD R2, R0
  ADC R3, R1
  ADC R4, R21

  MUL R5, R9
  ADD R3, R0
  ADC R4, R1

  ;--- add l4+h0 to l0 and h4 ---
  LDD R6, Z+0
  LDD R7, Z+1
  ADD R6, R14
  ADC R7, R15
  ADC R12, R16
  ADC R13, R17  
  ADC R14, R10
  ADC R15, R11
  ADC R16, R19
  ADC R17, R20
  ; store carry in R21
  
  ;--- process sign bit ---  
  CLR R8
  BLD R8, 0
  DEC R8

  ; merge carry and borrow
  ADC R21, R8
  MOV R0, R21
  ASR R0

  ; invert all bits or do nothing
  EOR R22, R8
  EOR R23, R8
  EOR R24, R8
  EOR R25, R8
  EOR R18, R8
  EOR R2,  R8
  EOR R3,  R8
  EOR R4,  R8
  ADD R8, R8 ; sets carry flag if R8 = 0xff

  ; add in M
  ADC R6,  R22
  ADC R7,  R23
  ADC R12, R24
  ADC R13, R25
  ADC R14, R18
  ADC R15, R2
  ADC R16, R3
  ADC R17, R4

  ; propagate carry/borrow
  ADC R10, R21
  ADC R11, R0
  ADC R19, R0
  ADC R20, R0

  STD Z+4, R6
  STD Z+5, R7
  STD Z+6, R12
  STD Z+7, R13

  
  MOVW R22, R14
  MOVW R24, R16
  MOV R18, R10
  MOV R21, R11
  ;h8...h15 stored in 22,23,24,25,18,21,19,20

  ;------ level 1: compute H ------

  ; init zero registers
  CLR R12
  CLR R13
  MOVW R14, R12
  MOVW R16, R12

  ;--- level 2: compute L ---
  LD R2, X+
  LD R3, X+
  LD R4, X+
  LD R5, X+
  LDD R6, Y+8
  LDD R7, Y+9
  LDD R8, Y+10
  LDD R9, Y+11
  
  MUL R2, R6
  MOVW R10, R0

  MUL R2, R7
  ADD R11, R0
  ADC R12, R1
  MUL R3, R6
  ADD R11, R0
  ADC R12, R1
  ADC R13, R17

  MUL R2, R8
  ADD R12, R0
  ADC R13, R1
  ADC R14, R17
  MUL R3, R7
  ADD R12, R0
  ADC R13, R1
  ADC R14, R17
  MUL R4, R6
  ADD R12, R0
  ADC R13, R1
  ADC R14, R17

  MUL R2, R9
  ADD R13, R0
  ADC R14, R1
  ADC R15, R17
  MUL R3, R8
  ADD R13, R0
  ADC R14, R1
  ADC R15, R17
  MUL R4, R7
  ADD R13, R0
  ADC R14, R1
  ADC R15, R17
  MUL R5, R6
  ADD R13, R0
  ADC R14, R1
  ADC R15, R17

  ; now add h0+l8 and h0+l12
  ADD R22, R10
  STD Z+16, R22
  ADC R23, R11
  STD Z+17, R23
  ADC R24, R12
  STD Z+18, R24
  ADC R25, R13
  STD Z+19, R25
  ADC R10, R18
  STD Z+20, R10 
  ADC R11, R21
  STD Z+21, R11
  ADC R12, R19
  ADC R13, R20
  ; store carry on stack
  SBC R0, R0
  PUSH R0
  CLR R20
  CLR R21
  
  ; continue
  MUL R3, R9
  ADD R14, R0
  ADC R15, R1
  ADC R16, R21
  MUL R4, R8
  ADD R14, R0
  ADC R15, R1
  ADC R16, R21
  MUL R5, R7
  ADD R14, R0
  ADC R15, R1
  ADC R16, R21

  MUL R4, R9
  ADD R15, R0
  ADC R16, R1
  ADC R17, R21
  MUL R5, R8
  ADD R15, R0
  ADC R16, R1
  ADC R17, R21

  MUL R5, R9
  ADD R16, R0
  ADC R17, R1

  ;--- load a4..a7 and b4..b7 ---
  MOVW R10, R20
  LD R18, X+
  LD R19, X+
  LD R20, X+
  ; R21 is loaded later
  LDD R22, Y+12
  LDD R23, Y+13
  LDD R24, Y+14
  LDD R25, Y+15

  ;--- level 2: compute H + (l3,l4,l5) ---
  MUL R18, R22
  ADD R14, R0
  ADC R15, R1
  ADC R16, R21
  ADC R11, R21  

  MUL R18, R23
  ADD R15, R0
  ADC R16, R1
  ADC R11, R21
  MUL R19, R22
  ADD R15, R0
  ADC R16, R1
  ADC R17, R11
  ADC R10, R21

  MUL R18, R24
  ADD R16, R0 
  ADC R17, R1
  ADC R10, R21
  MUL R19, R23
  ADD R16, R0
  ADC R17, R1
  ADC R10, R21
  MUL R20, R22
  ADD R16, R0
  ADC R17, R1
  ADC R10, R21

  CLR R11
  MUL R18, R25
  ADD R17, R0
  ADC R10, R1
  ADC R11, R21
  MUL R19, R24
  ADD R17, R0
  ADC R10, R1
  ADC R11, R21
  MUL R20, R23
  ADD R17, R0
  ADC R10, R1
  ADC R11, R21
  
  ;--- subtract a0-a4 ---
  SUB R2, R18
  SBC R3, R19
  SBC R4, R20
  ; load a7 to R18
  LD R18, X+
  SBC R5, R18
  ; 0xff if carry and 0x00 if no carry
  SBC R0, R0
  
  ;--- subtract b0-b4 ---
  SUB R6, R22
  SBC R7, R23
  SBC R8, R24
  SBC R9, R25
  ; 0xff if carry and 0x00 if no carry
  SBC R1, R1

  ;--- absolute values ---
  EOR R2, R0
  EOR R3, R0
  EOR R4, R0
  EOR R5, R0  
  EOR R6, R1
  EOR R7, R1
  EOR R8, R1
  EOR R9, R1  
  NEG R0
  NEG R1
  ADD R2, R0
  ADC R3, R21
  ADC R4, R21
  ADC R5, R21  
  ADD R6, R1
  ADC R7, R21
  ADC R8, R21
  ADC R9, R21  
  EOR R0, R1
  BST R0, 0   ; T <- R0[0]
  
  ;--- continue ---
  MUL R18, R22
  ADD R17, R0
  ADC R10, R1
  ADC R11, R21

  MUL R19, R25
  CLR R19
  ADD R10, R0
  ADC R11, R1
  ADC R19, R21
  MUL R20, R24
  ADD R10, R0
  ADC R11, R1
  ADC R19, R21
  MUL R18, R23
  ADD R10, R0
  ADC R11, R1
  ADC R19, R21

  MUL R20, R25
  CLR R20
  ADD R11, R0
  ADC R19, R1
  ADC R20, R21
  MUL R18, R24
  ADD R11, R0
  ADC R19, R1
  ADC R20, R21

  MUL R18, R25
  ADD R19, R0
  ADC R20, R1

  ;--- level 2: compute M ---
  CLR R24
  CLR R25
  CLR R18

  MUL R2, R6
  MOVW R22, R0

  MUL R2, R7
  ADD R23, R0
  ADC R24, R1
  MUL R3, R6
  ADD R23, R0
  ADC R24, R1
  ADC R25, R21

  MUL R2, R8
  ADD R24, R0
  ADC R25, R1
  ADC R18, R21
  MUL R3, R7
  ADD R24, R0
  ADC R25, R1
  ADC R18, R21
  MUL R4, R6
  ADD R24, R0
  ADC R25, R1
  ADC R18, R21

  MUL R2, R9
  CLR R2
  ADD R25, R0
  ADC R18, R1
  ADC R2, R21
  MUL R3, R8
  ADD R25, R0
  ADC R18, R1
  ADC R2, R21
  MUL R4, R7
  ADD R25, R0
  ADC R18, R1
  ADC R2, R21
  MUL R5, R6
  ADD R25, R0
  ADC R18, R1
  ADC R2, R21

  MUL R3, R9
  CLR R3
  ADD R18, R0
  ADC R2, R1
  ADC R3, R21
  MUL R4, R8
  ADD R18, R0
  ADC R2, R1
  ADC R3, R21
  MUL R5, R7
  ADD R18, R0
  ADC R2, R1
  ADC R3, R21

  MUL R4, R9
  CLR R4
  ADD R2, R0
  ADC R3, R1
  ADC R4, R21
  MUL R5, R8
  ADD R2, R0
  ADC R3, R1
  ADC R4, R21

  MUL R5, R9
  ADD R3, R0
  ADC R4, R1

  ;--- add l4+h0 to l0 and h4 ---
  LDD R6, Z+20
  LDD R7, Z+21
  ADD R6, R14
  ADC R7, R15
  ADC R12, R16
  ADC R13, R17  
  ADC R14, R10
  ADC R15, R11
  ADC R16, R19
  ADC R17, R20
  ; store carry in R21
  ADC R21, R21

  ;--- propagate carry ---  
  POP R0
  NEG R0
  ADD R14, R0
  CLR R0
  ADC R15, R0
  ADC R16, R0
  ADC R17, R0
  ;store carry in R21
  
  ;--- process sign bit ---
  CLR R8
  BLD R8, 0 ; R8[0] <- T flag 
  DEC R8

  ; merge carry and borrow
  ADC R21, R8
  MOV R0, R21
  ASR R0

  ; invert all bits or do nothing
  EOR R22, R8
  EOR R23, R8
  EOR R24, R8
  EOR R25, R8
  EOR R18, R8
  EOR R2,  R8
  EOR R3,  R8
  EOR R4,  R8
  ADD R8, R8 ; sets carry flag if R8 = 0xff

  ; add in M
  ADC R6,  R22
  ADC R7,  R23
  ADC R12, R24
  ADC R13, R25
  ADC R14, R18
  ADC R15, R2
  ADC R16, R3
  ADC R17, R4

  ; propagate carry/borrow
  ADC R10, R21
  ADC R11, R0
  ADC R19, R0
  ADC R20, R0

  STD Z+20, R6
  STD Z+21, R7
  STD Z+22, R12
  STD Z+23, R13
  STD Z+24, R14
  STD Z+25, R15
  STD Z+26, R16
  STD Z+27, R17
  STD Z+28, R10
  STD Z+29, R11
  STD Z+30, R19
  STD Z+31, R20

  ;------ level 1: subtract a0-a7 ------
  SBIW R26, 16
  LD R2, X+
  LD R3, X+
  LD R4, X+
  LD R5, X+
  LD R18, X+
  LD R19, X+
  LD R20, X+
  LD R21, X+
  LD R10, X+
  LD R11, X+
  LD R12, X+
  LD R13, X+
  LD R14, X+
  LD R15, X+
  LD R16, X+
  LD R17, X  
  SUB R2, R10
  SBC R3, R11
  SBC R4, R12
  SBC R5, R13
  SBC R18, R14
  SBC R19, R15
  SBC R20, R16
  SBC R21, R17
  ; 0xff if carry and 0x00 if no carry
  SBC R0, R0
  
  ;------ level 1: subtract b0-b7 ------
  LDD R6, Y+0
  LDD R7, Y+1
  LDD R8, Y+2
  LDD R9, Y+3
  LDD R22, Y+4
  LDD R23, Y+5
  LDD R24, Y+6
  LDD R25, Y+7
  LDD R10, Y+8
  LDD R11, Y+9
  LDD R12, Y+10
  LDD R13, Y+11
  LDD R14, Y+12
  LDD R15, Y+13
  LDD R16, Y+14
  LDD R17, Y+15
  SUB R6, R10
  SBC R7, R11
  SBC R8, R12
  SBC R9, R13
  SBC R22, R14
  SBC R23, R15
  SBC R24, R16
  SBC R25, R17
  ; 0xff if carry and 0x00 if no carry
  SBC R1, R1
    
  ;------ level 1: absolute values ------
  EOR R2, R0
  EOR R3, R0
  EOR R4, R0
  EOR R5, R0
  EOR R18, R0
  EOR R19, R0
  EOR R20, R0
  EOR R21, R0
  EOR R6, R1
  EOR R7, R1
  EOR R8, R1
  EOR R9, R1
  EOR R22, R1
  EOR R23, R1
  EOR R24, R1
  EOR R25, R1
  NEG R0
  NEG R1
  CLR R26
  CLR R27
  ADD R2, R0
  ADC R3, R26
  ADC R4, R26
  ADC R5, R26
  ADC R18, R26
  ADC R19, R26
  ADC R20, R26
  ADC R21, R26  
  ADD R6, R1
  ADC R7, R26
  ADC R8, R26
  ADC R9, R26
  ADC R22, R26
  ADC R23, R26
  ADC R24, R26
  ADC R25, R26  
  EOR R0, R1
  PUSH R0

  ;------ level 1: compute M ------
  MOVW R16, R26

  MUL R2, R8 ;a0*b2
  MOVW R12, R0
  MUL R2, R6 ;a0*b0
  MOVW R10, R0
  MUL R2, R7 ;a0*b1
  ADD R11, R0
  ADC R12, R1
  ADC R13, R26
  MUL R3, R9 ;a1*b3
  MOVW R14, R0

  MUL R2, R9 ;a0*b3
  MOVW R28, R0
  MUL R3, R6 ;a1*b0
  ADD R11, R0
  ADC R12, R1
  ADC R13, R28
  ADC R29, R26
  MUL R3, R7 ;a1*b1
  ADD R12, R0
  ADC R13, R1
  ADC R29, R26
  MUL R4, R9 ;a2*b3
  ADD R14, R29
  ADC R15, R0
  ADC R16, R1

  MUL R4, R8 ;a2*b2
  MOVW R28, R0
  MUL R4, R6 ;a2*b0
  ADD R12, R0
  ADC R13, R1
  ADC R14, R28
  ADC R29, R26
  MUL R3, R8 ;a1*b2
  ADD R13, R0
  ADC R14, R1
  ADC R29, R26
  MUL R5, R9 ;a3*b3
  ADD R15, R29
  ADC R16, R0
  ADC R17, R1

  MUL R5, R7 ;a3*b1
  MOVW R28, R0
  MUL R4, R7 ;a2*b1
  ADD R13, R0
  ADC R28, R1
  ADC R29, R26
  MUL R5, R6 ;a3*b0
  ADD R13, R0
  ADC R28, R1
  ADC R29, R26
  MUL R5, R8 ;a3*b2
  ADD R14, R28
  ADC R0, R29
  ADC R1, R26
  ADD R15, R0
  ADC R16, R1
  ADC R17, R26

  ;--- subtract a0-a4 ---  
  SUB R2, R18
  SBC R3, R19
  SBC R4, R20
  SBC R5, R21
  ; 0xff if carry and 0x00 if no carry
  SBC R0, R0
  
  ;--- subtract b0-b4 ---  
  SUB R6, R22
  SBC R7, R23
  SBC R8, R24
  SBC R9, R25
  ; 0xff if carry and 0x00 if no carry
  SBC R1, R1

  ;--- absolute values ---  
  EOR R2, R0
  EOR R3, R0
  EOR R4, R0
  EOR R5, R0  
  EOR R6, R1
  EOR R7, R1
  EOR R8, R1
  EOR R9, R1  
  NEG R0
  NEG R1
  ADD R2, R0
  ADC R3, R26
  ADC R4, R26
  ADC R5, R26  
  ADD R6, R1
  ADC R7, R26
  ADC R8, R26
  ADC R9, R26  
  EOR R0, R1
  BST R0, 0    ; T <- R0[0] 
  
  ;--- level 2: compute H + (l3,l4,l5) ---
  MUL R18, R24 ;a0*b2
  MOVW R28, R0
  MUL R18, R22 ;a0*b0
  ADD R14, R0
  ADC R15, R1
  ADC R16, R28
  ADC R29, R26
  MUL R18, R23 ;a0*b1
  ADD R15, R0
  ADC R16, R1
  ADC R29, R26
  MUL R19, R25 ;a1*b3
  ADD R17, R29
  ADC R26, R0
  ADC R27, R1

  MUL R18, R25 ;a0*b3
  MOVW R28, R0
  MUL R19, R22 ;a1*b0
  ADD R15, R0
  ADC R16, R1
  ADC R17, R28
  CLR R18
  ADC R29, R18
  MUL R19, R23 ;a1*b1
  ADD R16, R0
  ADC R17, R1
  ADC R29, R18
  MUL R20, R25 ;a2*b3
  ADD R26, R29
  ADC R27, R0
  CLR R18
  ADC R18, R1

  MUL R20, R24 ;a2*b2
  MOVW R28, R0
  MUL R20, R22 ;a2*b0
  ADD R16, R0
  ADC R17, R1
  ADC R26, R28
  CLR R0
  ADC R29, R0
  MUL R19, R24 ;a1*b2
  ADD R17, R0
  ADC R26, R1
  CLR R19
  ADC R29, R19
  MUL R21, R25 ;a3*b3
  ADD R27, R29
  ADC R18, R0
  CLR R25
  ADC R25, R1
  
  MUL R21, R23 ;a3*b1
  MOVW R28,R0
  MUL R20, R23 ;a2*b1
  ADD R17, R0
  ADC R28, R1
  ADC R29, R19
  MUL R21, R22 ;a3*b0
  ADD R17, R0
  ADC R28, R1
  ADC R29, R19
  MUL R21, R24 ;a3*b2
  ADD R26, R28
  ADC R0, R29
  ADC R1, R19
  ADD R27, R0
  ADC R18, R1
  ADC R25, R19

  ;--- level 2: compute M ---
  MUL R2, R8 ;a0*b2
  MOVW R22, R0
  MUL R2, R6 ;a0*b0
  MOVW R20, R0
  MUL R2, R7 ;a0*b1
  ADD R21, R0
  ADC R22, R1
  ADC R23, R19
  MUL R3, R9 ;a1*b3
  MOV R24, R0
  MOV R0, R2
  MOV R2, R1

  MUL R0, R9 ;a0*b3
  MOVW R28, R0
  MUL R3, R6 ;a1*b0
  ADD R21, R0
  ADC R22, R1
  ADC R23, R28
  ADC R29, R19
  MUL R3, R7 ;a1*b1
  ADD R22, R0
  ADC R23, R1
  ADC R29, R19
  MUL R4, R9 ;a2*b3
  ADD R24, R29
  ADC R2, R0
  ADC R19, R1  

  MUL R4, R8 ;a2*b2
  MOVW R28, R0
  MUL R4, R6 ;a2*b0
  ADD R22, R0
  ADC R23, R1
  ADC R24, R28
  CLR R0
  ADC R29, R0
  MUL R3, R8 ;a1*b2
  ADD R23, R0
  ADC R24, R1
  CLR R3
  ADC R29, R3
  MUL R5, R9 ;a3*b3
  ADD R2, R29
  ADC R19, R0
  CLR R9
  ADC R9, R1

  MUL R5, R7 ;a3*b1
  MOVW R28, R0
  MUL R4, R7 ;a2*b1
  ADD R23, R0
  ADC R28, R1
  ADC R29, R3
  MUL R5, R6 ;a3*b0
  ADD R23, R0
  ADC R28, R1
  ADC R29, R3
  MUL R5, R8 ;a3*b2
  ADD R24, R28
  ADC R0, R29
  ADC R1, R3
  ADD R2, R0
  ADC R19, R1
  ADC R9, R3

  ;--- add l4+h0 to l0 and h4 ---
  MOVW R4, R10
  MOVW R6, R12
  ADD R10, R14
  ADC R11, R15
  ADC R12, R16
  ADC R13, R17
  ADC R14, R26
  ADC R15, R27
  ADC R16, R18
  ADC R17, R25
  ; store carry in R3
  
  ;--- process sign bit ---
  CLR R0
  BLD R0, 0  ; R0[0] <- T
  DEC R0

  ; merge carry and borrow
  ADC R3, R0
  MOV R1, R3
  ASR R1

  ; invert all bits or do nothing
  EOR R20, R0
  EOR R21, R0
  EOR R22, R0
  EOR R23, R0
  EOR R24, R0
  EOR R2,  R0
  EOR R19, R0
  EOR R9,  R0
  ADD R0, R0 ; sets carry flag if R0 = 0xff

  ; add in M
  ADC R10, R20
  ADC R11, R21
  ADC R12, R22
  ADC R13, R23
  ADC R14, R24
  ADC R15, R2
  ADC R16, R19
  ADC R17, R9

  ; propagate carry/borrow
  ADC R26, R3
  ADC R27, R1
  ADC R18, R1
  ADC R25, R1

  MOV R19, R25

  ;------ level 1: combine L, H, and M ------
  LDD R20, Z+0
  LDD R21, Z+1
  LDD R22, Z+2
  LDD R23, Z+3
  LDD R24, Z+4  
  LDD R25, Z+5
  LDD R8, Z+6 
  LDD R9, Z+7

  ;--- process sign bit ---  
  POP R0  
  DEC R0  

  MOV R1, R0   
  ASR R1     ; shift right but MSB is held unchanged (division by 2 working for 7-bit magnitude signed and unsigned numbers)

  ; invert all bits or do nothing
  EOR R4, R0
  EOR R5, R0
  EOR R6, R0
  EOR R7, R0
  EOR R10, R0
  EOR R11, R0
  EOR R12, R0
  EOR R13, R0
  EOR R14, R0
  EOR R15, R0
  EOR R16, R0
  EOR R17, R0
  EOR R26, R0
  EOR R27, R0
  EOR R18, R0
  EOR R19, R0
  MOV R28, R0
  ADD R28, R28 ; sets carry flag if R28 = 0xff
  ; add in M
  ADC R20, R4
  ADC R21, R5
  ADC R22, R6
  ADC R23, R7
  ADC R24, R10
  ADC R25, R11
  ADC R8,  R12
  ADC R9,  R13
  
  ; store carry in R28
  CLR R28
  ADC R28, R28 ; R28 = 0+0+C

  LDD R2, Z+16
  LDD R3, Z+17
  ADD R20, R2
  ADC R21, R3
  STD Z+8, R20
  STD Z+9, R21
  MOVW R20, R2
  LDD R2, Z+18
  LDD R3, Z+19
  ADC R22, R2
  ADC R23, R3
  STD Z+10, R22
  STD Z+11, R23
  MOVW R22, R2
  LDD R2, Z+20
  LDD R3, Z+21
  ADC R24, R2
  ADC R25, R3
  STD Z+12, R24
  STD Z+13, R25
  MOVW R24, R2
  LDD R2, Z+22
  LDD R3, Z+23
  ADC R8, R2
  ADC R9, R3
  STD Z+14, R8
  STD Z+15, R9
  MOVW R8, R2
  
  ; store carry in R2
  CLR R2
  ADC R2, R2    ; R2 = 0+0+C

  LSR R28       ; Recover carry from R28
  ADC R20, R14
  ADC R21, R15
  ADC R22, R16
  ADC R23, R17
  ADC R24, R26
  ADC R25, R27
  ADC R8, R18
  ADC R9, R19
  
  ; store carry in R1:R0
  ADC R0, R28 
  ADC R1, R28 

  LDD R4, Z+24
  LDD R5, Z+25
  LDD R6, Z+26
  LDD R7, Z+27
  LDD R10, Z+28
  LDD R11, Z+29
  LDD R12, Z+30
  LDD R16, Z+31
  

  LSR R2      ; recover the carry stored in R2
  ADC R20, R4
  ADC R21, R5
  ADC R22, R6
  ADC R23, R7
  ADC R24, R10
  ADC R25, R11
  ADC R8,  R12
  ADC R9,  R16
  
  ; store carry in R1:R0
  ADC R0, R2 ; R0 = R0 + 0 + c 
  ADC R1, R2 ; R1 = R1 + 0 + c 

  ;--- propagate carry to end ---
  ADD R4, R0
  ADC R5, R1
  ADC R6, R1
  ADC R7, R1
  ADC R10, R1
  ADC R11, R1
  ADC R12, R1
  ADC R16, R1

;  R4?R15 and R24?R29
  /////////////////////////////////////////////////////////////////////////////REDUCTION

  MOVW R28, R30 // Product c; Y = &c
  POP R31
  POP R30


// Load MSByte of cL where c = a*b = cH*2^127 + cL (recall that c mod 2^127-1 = cH + cL)
  LDD T0, Y+15  ; T0 <- cL[15]
  MOV T1, T0    ; T1 <- cL[15]
  ANDI T0, 0X7F ; T0 <- cL[15] & 0x7f (MSB of cL[15] belongs to cH thus it is cleared in here)
  
// Lets prepare cH by computing ((c[31] c[30] ... c[16]) << 1 ) XOR 0x0cL[15][7]
  ADD T1, T1 ; Carry = MSB of cL[15]
  ADC B0, B0 ; cH[0] <- cH[0] << 1 OR carry
  ADC B1, B1
  ADC B2, B2
  ADC B3, B3
  ADC B4, B4
  ADC B5, B5
  ADC B6, B6
  ADC B7, B7
  ADC B8, B8
  ADC B9, B9
  ADC B10, B10
  ADC B11, B11
  ADC B12, B12
  ADC B13, B13
  ADC B14, B14
  ADC R16, R16
  
  // cH is now prepared to be added to cL
  
  LD T1, Y+   ; T1 <- cL[0]
  ADD B0, T1  ; cH[0] <- cH[0] + cL[0]
  LD T1, Y+   
  ADC B1, T1  ; cH[1] <- cH[1] + cL[1] + C
  LD T1, Y+  
  ADC B2, T1  ; cH[2] <- cH[2] + cL[2] + C
  LD T1, Y+
  ADC B3, T1  ; cH[3] <- cH[3] + cL[3] + C
  LD T1, Y+
  ADC B4, T1  ; cH[4] <- cH[4] + cL[4] + C
  LD T1, Y+
  ADC B5, T1  ; cH[5] <- cH[5] + cL[5] + C
  LD T1, Y+
  ADC B6, T1  ; cH[6] <- cH[6] + cL[6] + C
  LD T1, Y+
  ADC B7, T1  ; cH[7] <- cH[7] + cL[7] + C
  LD T1, Y+
  ADC B8, T1  ; cH[8] <- cH[8] + cL[8] + C
  LD T1, Y+
  ADC B9, T1  ; cH[9] <- cH[9] + cL[9] + C
  LD T1, Y+
  ADC B10, T1 ;cH[10] <- cH[10] + cL[10] + C
  LD T1, Y+
  ADC B11, T1 ;cH[11] <- cH[11] + cL[11] + C
  LD T1, Y+   
  ADC B12, T1 ;cH[12] <- cH[12] + cL[12] + C
  LD T1, Y+
  ADC B13, T1 ;cH[13] <- cH[13] + cL[13] + C
  LD T1, Y+
  ADC B14, T1 ;cH[14] <- cH[14] + cL[14] + C
  ADC R16, T0 ;cH[15] <- cH[15] + cL[15] + C
  
  // Now cH contians contains the result with a possible carry which should be reduced
  // The carry (MSB of cH[15]) is equivalent to C*2^127 which reduced mod 2^127-1 is just C*2^0
  // Therefore, C must be added to cH
  
  MOV T0, R16
  ANDI R16, 0X7F ; clears the MSB
  
  CLR ZERO
  
  ADC T0, T0   ; C <- cH[15][7]
  
  ADC B0, ZERO ; cH[0] <- cH[0] + C
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
  ADC R16, ZERO ; cH[15] <- cH[15] + C
    
  
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
  ST Z+, R16

  IN R30, 0x3D
  IN R31, 0x3E
  ADIW R30, 34
  OUT 0x3D, R30
  OUT 0x3E, R31

////////////////////////
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
  POP R7
  POP R6
  POP R5
  POP R4
  
  RET

END
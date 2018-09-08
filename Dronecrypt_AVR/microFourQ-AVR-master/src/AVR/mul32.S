//////////////////////////////////////////////////////////////////////////////
// 32-bit multiprecision multiplication                                     //
//                                                                          //
// Authors: Zhe Liu, Geovandro Pereira, Hwajeong Seo                        //
//////////////////////////////////////////////////////////////////////////////

NAME mul32           ; module name
PUBLIC mul32         ; make the main label vissible outside this module
RSEG CODE            ; place program in 'CODE' segment

; operand b: r21:r20
; operand a: r23:r22
; operand r: r25:r24
#define A0 R2
#define A1 R3
#define A2 R4
#define A3 R5

#define B0 R6
#define B1 R7
#define B2 R8
#define B3 R9

#define C0 R16
#define C1 R17
#define C2 R18
#define C3 R19
#define C4 R20
#define C5 R21
#define C6 R22
#define C7 R23
#define ZERO R24


mul32:
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

  MOVW R30, R20
  MOVW R28, R16
  MOVW R26, R18

  CLR ZERO

  LD   A0, X+	
  LD   A1, X+	
  LD   A2, X+	
  LD   A3, X+	

  LD   B0, Y+	
  LD   B1, Y+	
  LD   B2, Y+	
  LD   B3, Y+	

  //1
  CLR C2
  MUL A0, B0  
  MOVW C0, R0
  
  //2
  CLR C3
  MUL A0, B1
  ADD C1, R0
  ADC C2, R1
  ADC C3, ZERO

  MUL A1, B0
  ADD C1, R0
  ADC C2, R1
  ADC C3, ZERO

  //3
  CLR C4
  MUL A0, B2
  ADD C2, R0
  ADC C3, R1
  ADC C4, ZERO

  MUL A1, B1
  ADD C2, R0
  ADC C3, R1
  ADC C4, ZERO

  MUL A2, B0
  ADD C2, R0
  ADC C3, R1
  ADC C4, ZERO
  
  
  //4
  CLR C5
  MUL A0, B3
  ADD C3, R0
  ADC C4, R1
  ADC C5, ZERO

  MUL A1, B2
  ADD C3, R0
  ADC C4, R1
  ADC C5, ZERO
  
  MUL A2, B1
  ADD C3, R0
  ADC C4, R1
  ADC C5, ZERO
  
  MUL A3, B0
  ADD C3, R0
  ADC C4, R1
  ADC C5, ZERO

  //5
  CLR C6
  MUL A1, B3
  ADD C4, R0
  ADC C5, R1
  ADC C6, ZERO

  MUL A2, B2
  ADD C4, R0
  ADC C5, R1
  ADC C6, ZERO
  
  MUL A3, B1
  ADD C4, R0
  ADC C5, R1
  ADC C6, ZERO
  
  //6
  CLR C7
  MUL A2, B3
  ADD C5, R0
  ADC C6, R1
  ADC C7, ZERO

  MUL A3, B2
  ADD C5, R0
  ADC C6, R1
  ADC C7, ZERO
  
  //7
  MUL A3, B3
  ADD C6, R0
  ADC C7, R1
  
  ST Z+, C0
  ST Z+, C1
  ST Z+, C2
  ST Z+, C3
  
  ST Z+, C4
  ST Z+, C5
  ST Z+, C6
  ST Z+, C7
  ;  R4?R15 and R24?R29
  
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

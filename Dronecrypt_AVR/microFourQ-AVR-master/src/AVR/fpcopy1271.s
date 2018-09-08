//////////////////////////////////////////////////////////////////////////////
// Field element copy R <- A                                                //
//                                                                          //
// Authors: Zhe Liu, Hwajeong Seo                                           //        
//////////////////////////////////////////////////////////////////////////////

; operand b: r21:r20
; operand a: r23:r22
; operand r: r25:r16
NAME fpcopy1271            ; module name
PUBLIC fpcopy1271          ; make the main label vissible outside this module
RSEG CODE                  ; place program in 'CODE' segment

;  R4–R15 and R24–R29
fpcopy1271:
  
  PUSH R26
  PUSH R27
  
  MOVW R30, R18 // R
  MOVW R26, R16 // A
    
  //Load A: 16 bytes
  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16

  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16

  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16

  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16
  LD R16, X+
  ST Z+, R16
  
  
  POP R27
  POP R26
  
  RET
  
  END
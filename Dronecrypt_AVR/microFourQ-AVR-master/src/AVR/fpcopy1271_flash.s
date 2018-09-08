//////////////////////////////////////////////////////////////////////////////
// Copy Field element from flash                                            //
//                                                                          //
// Authors: Zhe Liu, Hwajeong Seo                                           //        
﻿//////////////////////////////////////////////////////////////////////////////

; operand b: r21:r20
; operand a: r23:r22
; operand r: r25:r16
NAME fpcopy1271_flash            ; module name
PUBLIC fpcopy1271_flash          ; make the main label vissible outside this module
RSEG CODE                   ; place program in 'CODE' segment
;  R4–R15 and R24–R29
fpcopy1271_flash:
  
  PUSH R26
  PUSH R27
  
  MOVW R26, R18 // R
  MOVW R30, R16 // A

  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16

  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16

  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16

  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16
  LPM R16, Z+
  ST X+, R16
  
  POP R27
  POP R26
  
  RET
  
  END
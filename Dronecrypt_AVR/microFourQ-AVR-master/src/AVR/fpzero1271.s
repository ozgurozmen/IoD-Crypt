//////////////////////////////////////////////////////////////////////////////
// Assigns zero to element to r: r = 0                                     //
//                                                                          //
// Authors: Zhe Liu, Hwajeong Seo                                           //        
//////////////////////////////////////////////////////////////////////////////

NAME fpzero1271            ; module name
PUBLIC fpzero1271          ; make the main label vissible outside this module
RSEG CODE                   ; place program in 'CODE' segment
;  R4–R15 and R24–R29
; operand b: r21:r20
; operand a: r23:r22
; operand r: r25:r24

fpzero1271:
  
  MOVW R30, R16 // A
  CLR R16
    
  ST Z+, R16
  ST Z+, R16
  ST Z+, R16
  ST Z+, R16

  ST Z+, R16
  ST Z+, R16
  ST Z+, R16
  ST Z+, R16

  ST Z+, R16
  ST Z+, R16
  ST Z+, R16
  ST Z+, R16

  ST Z+, R16
  ST Z+, R16
  ST Z+, R16
  ST Z+, R16

      
  RET
END
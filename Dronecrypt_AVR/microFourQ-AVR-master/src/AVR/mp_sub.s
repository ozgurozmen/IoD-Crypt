//////////////////////////////////////////////////////////////////////////////
// Subtraction of two Multiple-Precision Integers on 8-bit AVR Processors   //
// Version 1.0.0 (2015-01-26) by Johann Groszschaedl <www.cryptography.lu>  //
// Copyright (C) 2015 University of Luxembourg, Luxembourg <www.uni.lu>     //
// License: GPLv3 (see LICENSE file), other licenses available upon request //
//////////////////////////////////////////////////////////////////////////////

#define XL R26
#define YL R28
#define ZL R30

#define LEN R22
#define A0 R19
#define B0 R20 
#define LOOP_STOP R21


// if A<B the return value is 1
// if A>=B return 0


NAME mp_sub            ; module name
PUBLIC mp_sub          ; make the main label vissible outside this module
RSEG CODE                   ; place program in 'CODE' segment
;  R4?R15 and R24?R29
mp_sub:
    
    
    PUSH R26
    PUSH R27
    PUSH R28
    PUSH R29
    
    // Parameters: R, A, B, len
    
    MOVW ZL, R20
    MOVW XL, R16
    MOVW YL, R18
    LSL  LEN            // double LEN
    LSL  LEN            // LEN contains now the number of bytes of array A
    
    MOV  LOOP_STOP, XL
    ADD  LOOP_STOP, LEN
    CLC
    
LOOP:
    LD   A0, X+
    LD   B0, Y+
    SBC  A0, B0
    ST   Z+, A0
    LD   A0, X+
    LD   B0, Y+
    SBC  A0, B0
    ST   Z+, A0
    LD   A0, X+
    LD   B0, Y+
    SBC  A0, B0
    ST   Z+, A0
    LD   A0, X+
    LD   B0, Y+
    SBC  A0, B0
    ST   Z+, A0
    CPSE LOOP_STOP, XL
    RJMP LOOP
    
    // return carry
    CLR  R16
    ADC  R16, R16
    CLR  R17
    
    POP  R29
    POP  R28
    POP R27
    POP R26
    
    RET
    
END
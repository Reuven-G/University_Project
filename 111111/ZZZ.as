; Sample file for Assembler Testing - Maman 14
.entry LIST
.extern W

; Macro definition test
mcro MY_MACRO
    add r3, LIST
    prn #48
mcroend

MAIN:   mov r3, LIST
        
        ; Using a macro
        MY_MACRO
        
        lea STR, r6
        inc r6
        
        ; Relative addressing test
        bne %END
        
        ; External usage test
        sub W, r1
        
        ; Instruction with two registers (shorthand test)
        mov r1, r2
        
        ; Stop instruction
        stop

; Data directives test
STR:    .string "abcd"
LIST:   .data 6, -9, 15
K:      .data 22

END:    stop

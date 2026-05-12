.entry LIST
.extern W
MAIN:   add r1, LIST
        mov #48, r5
LOOP:   jmp %END
        prn # -5
        sub r1, r4
        inc W
        mov LIST, r3
        bne LOOP
END:    stop
LIST:   .data 6, -9
        .string "abc"

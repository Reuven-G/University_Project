; test4
.entry START
.extern LIB
mcro CALL_LIB
    jsr LIB
mcroend
START:  CALL_LIB
        mov r0, r1
        stop
NAME:   .string "Hi"
NUM:    .data 7

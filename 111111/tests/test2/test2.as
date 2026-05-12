; test2
mcro GREET
    prn #72
    prn #105
mcroend
MAIN:   prn #72
    prn #105
        bne %MAIN
        stop
MSG:    .string "Hi"

; test3.as
.extern IO
mcro PRINT_IO
    prn IO
mcroend
START:  PRINT_IO
        inc r3
        PRINT_IO
        stop

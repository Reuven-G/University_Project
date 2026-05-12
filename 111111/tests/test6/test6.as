; test6
.entry START
BAD LABEL : mov r1, r2
data 1	0, 20
mov 	r1, r2$
add 	r1,, r2
EMPTY: 	.string ""
.extern mov

START: jmp #5

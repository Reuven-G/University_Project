; test5
.entry 	START
.extern EXTERN_LABEL
r1:	mov r2, r3
hello 	r1, r2
add 	#5, #10
VAL: 	.data 2048
STR: 	.string incomplete

START: stop extra_text

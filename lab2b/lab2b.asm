;number to print in decimal is in R3.
;it will be positive.
.ORIG x3000

	LD R3, TEST ; load value for testing
	AND R4, R4, #0 ; clear R4
	ADD R4, R4, #10 ; R4 = 10 for dividing #'s by 10
MAIN	
	JSR DIV
	AND R3, R3, #0
	ADD R3, R3, R0 ; move previous quotient to R3
	AND R0, R0, #0
	ADD R0, R0, R1 ; move remainder to R0
	JSR PUSH
	ADD R3, R3, #0 ; activates to set CC
	BRp MAIN

PRINT
	JSR POP
	ADD R5, R5, #0
	BRp DONE
	LD R2, ASCII_0
	ADD R0, R0, R2
	OUT
	BRnzp PRINT

	; JSR POP, check R5, if 1, we're finished
	; if R5==0, R0 + '0'
	; PRINT (OUT)
	; go back to PRINT loop

DONE
	HALT

TEST	.FILL x0ABC

ASCII_0 .FILL x0030
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0-quotient, R1-remainder
DIV
	ST R2, save2
	AND R0, R0, #0
	ADD R1, R3, #0
	NOT R2, R4
	ADD R2, R2, #1 ; R2 = -R4 for subtraction
DLOOP	
	ADD R0, R0, #1
	ADD R1, R1, R2 ; R1 + R2 (R1 - R4)
	BRzp DLOOP
	ADD R0, R0, #-1
	ADD R1, R1, R4
	LD R2, save2
RET

; Register save locations
save0	.BLKW #1
save1	.BLKW #1	
save2	.BLKW #1	
save3	.BLKW #1	
save4	.BLKW #1
save5	.BLKW #1	
save6	.BLKW #1	
save7	.BLKW #1


;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;

.END

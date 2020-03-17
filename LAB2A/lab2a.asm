.ORIG x3000
; Write code to read in characters and echo them
; till a newline character is entered.

KIN		GETC ; gets char from keyboard
		OUT ; echoes entered character to screen
		LD R1, SPACE ; checks if space entered
		NOT R1, R1
		ADD R1, R1, #1
		ADD R1, R0, R1
		BRz KIN ; get a new char
		LD R1, NEW_LINE
		NOT R1, R1
		ADD R1, R1, #1
		ADD R1, R0, R1
		BRz END ; end program if enter pressed
		LD R1, CHAR_RETURN
		NOT R1, R1
		ADD R1, R1, #1
		ADD R1, R0, R1
		BRz END ; end program if enter pressed
		JSR IS_BALANCED
		ADD R5, R5, #0
		BRn FAIL
		BRzp KIN
END		JSR POP
		ADD R5, R5, #0
		BRz FAIL
		AND R5, R5, #0
		ADD R5, R5, #1
FAIL	HALT


 
SPACE   		.FILL x0020
NEW_LINE        .FILL x000A
CHAR_RETURN     .FILL x000D
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;if ( push onto stack if ) pop from stack and check if popped value is (
;input - R0 holds the input
;output - R5 set to -1 if unbalanced. else not modified.

IS_BALANCED
ST R1, save1
ST R7, save7
LD R1, NEG_OPEN
ADD R1, R0, R1
BRz OP
LD R1, NEG_CLOSE
ADD R1, R0, R1
BRz CP
EXIT ; exit label
LD R1, save1
LD R7, save7
RET
OP ; '(' label
JSR PUSH
BRnzp EXIT
CP ; ')' label
JSR POP
ADD R5, R5, #0
BRnz PRO
ADD R5, R5, #-2
PRO
BRnzp EXIT

NEG_OPEN .FILL xFFD8
NEG_CLOSE .FILL xFFD7

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
        ST R3, PUSH_SaveR3      ;save R3
        ST R4, PUSH_SaveR4      ;save R4
        AND R5, R5, #0          ;
        LD R3, STACK_END        ;
        LD R4, STACk_TOP        ;
        ADD R3, R3, #-1         ;
        NOT R3, R3              ;
        ADD R3, R3, #1          ;
        ADD R3, R3, R4          ;
        BRz OVERFLOW            ;stack is full
        STR R0, R4, #0          ;no overflow, store value in the stack
        ADD R4, R4, #-1         ;move top of the stack
        ST R4, STACK_TOP        ;store top of stack pointer
        BRnzp DONE_PUSH         ;
OVERFLOW
        ADD R5, R5, #1          ;
DONE_PUSH
        LD R3, PUSH_SaveR3      ;
        LD R4, PUSH_SaveR4      ;
        RET


PUSH_SaveR3     .BLKW #1        ;
PUSH_SaveR4     .BLKW #1        ;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP
        ST R3, POP_SaveR3       ;save R3
        ST R4, POP_SaveR4       ;save R3
        AND R5, R5, #0          ;clear R5
        LD R3, STACK_START      ;
        LD R4, STACK_TOP        ;
        NOT R3, R3              ;
        ADD R3, R3, #1          ;
        ADD R3, R3, R4          ;
        BRz UNDERFLOW           ;
        ADD R4, R4, #1          ;
        LDR R0, R4, #0          ;
        ST R4, STACK_TOP        ;
        BRnzp DONE_POP          ;
UNDERFLOW
        ADD R5, R5, #1          ;
DONE_POP
        LD R3, POP_SaveR3       ;
        LD R4, POP_SaveR4       ;
        RET


POP_SaveR3      .BLKW #1        ;
POP_SaveR4      .BLKW #1        ;

STACK_END       .FILL x3FF0     ;
STACK_START     .FILL x4000     ;
STACK_TOP       .FILL x4000     ;

.END


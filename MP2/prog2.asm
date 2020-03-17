; MP2: Written by Junyeob, Murugan, and Jason
;
; This program operates in a loop, waiting for user input
; When something is input, the loop checks if it is a space or semicolon
; If it is a space, it branches back to the top of the loop
; If it is a semicolon, it branches to the end routine
; In the end routine, two pops are performed.
; If the first is successful and the other is unsuccessful, the expression is valid
; The first pop is the answer, which is printed in hex and moved to R5
; If not, the expression is invalid and invalid expression is printed
; If it is neither a space or semicolon, control is passed to the EVALUATE subroutine

.ORIG x3000
	
; Main Loop
; R0 - Character input, and echo output
; R1 - Register used for comparisons
; R2 - Unused in main loop
; R3 - Holds answer for printing in hex
; R4 - Unused in main loop
; R5 - Holds final solution
; R6 - Unused in main loop
; R7 - Used to hold memory addresses for JSR

STARTING
	GETC
	OUT			; Get and echo character entered
	LD R1, SPACE_INV	; checks if space entered
	ADD R1, R0, R1
	BRz STARTING		; get a new char if space entered
	LD R1, SEMI_INV 	; checks if semicolon entered
	ADD R1, R0, R1
	BRz END			; end program if semicolon entered
	JSR EVALUATE		; Calls the evaluate subroutine
	BRnzp STARTING		; Goes back to the top to get new char
END
	JSR POP			; first POP to draw answer
	ADD R5, R5, #0
	BRp MAIN_INVAL
	JSR POP			; second POP to make sure stack is empty
	ADD R5, R5, #0
	BRnz MAIN_INVAL
	ADD R3, R0, #0		; Move answer to R3
	JSR PRINT_HEX		; prints the answer in hexadecimal
	ADD R5, R0, #0		; moves answer to R5
	HALT

MAIN_INVAL
	JSR INVALID_INPUT	; Jumps to printing “Invalid Expression”
	
SPACE_INV	.FILL	xFFE0
SEMI_INV	.FILL	xFFC5


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This subroutine prints the value in R3 as 4 hex digits
; It works the same as the code in MP1
; This following description is copied from MP1
; In order to convert from binary to ASCII hex, we left shift four bits at a time from the value into another register.
; Once we have four bits, we subtract decimal 10 from those four bits in order to determine if its a letter or a number.
; If it’s a letter, we add the remainder to ASCII A and print that. If a number, we add the original value to ASCII 0 and print that.
; We repeat that process four times in order to print all 4 ASCII digits.

; Register Table
; R0 - Holds the character to print out
; R1 - Number of characters printed
; R2 - Bit counter for each character
; R3 - The register we are printing
; R4 - The 4 bits we are going to print out
; R5 - Not Used
; R6 - Not Used
; R7 - Holds JSR and RET info

;R3- value to print in hexadecimal
PRINT_HEX
ST R0, HEX_SAVE_R0
ST R1, HEX_SAVE_R1
ST R2, HEX_SAVE_R2
ST R3, HEX_SAVE_R3
ST R4, HEX_SAVE_R4
ST R7, HEX_SAVE_R7
AND R1, R1, #0		    	; clear R1
ADD R1, R1, #4		    	; initialize digit counter to 4
HEX_OUTER
ADD R1, R1, #0		    	; activate digit counter to nzp
BRz HEX_FIN 		    	; printed <4 chars?
AND R2, R2, #0			; clear R2
AND R4, R4, #0			; initialize digit
ADD R2, R2, #4			; initialize bit counter to 4
HEX_INNER
ADD R2, R2, #0			; activate bit counter into nzp
BRz HEX_PRINT			; got <4 bits from R3?
ADD R4, R4, R4			; shift digit left
ADD R3, R3, #0			; activate R3 into nzp
BRzp HEX_FALL
ADD R4, R4, #1			; add 1 to digit
HEX_FALL
ADD R3, R3, R3			; shift R3 left
ADD R2, R2, #-1			; decrement bit counter
BRnzp HEX_INNER
HEX_PRINT
ADD R4, R4, #0			; activate R4 into nzp
ADD R0, R4, #-9
BRnz HEX_TRUE			; is digit <= 9?
LD R0, HEX_ASCII_AL
ADD R0, R0, #-10
ADD R0, R4, R0
BRnzp HEX_NEXT
HEX_TRUE
LD R0, HEX_ASCII_ZERO
ADD R0, R4, R0
HEX_NEXT
OUT				; prints out a character
ADD R1, R1, #-1			; decrement digit counter
BRnzp HEX_OUTER
HEX_FIN
LD R0, HEX_SAVE_R0
LD R1, HEX_SAVE_R1
LD R2, HEX_SAVE_R2
LD R3, HEX_SAVE_R3
LD R4, HEX_SAVE_R4
LD R7, HEX_SAVE_R7
RET

HEX_ASCII_ZERO		.FILL x0030 ; ASCII 0
HEX_ASCII_AL		.FILL x0041 ; ASCII A

HEX_SAVE_R0		.BLKW #1
HEX_SAVE_R1		.BLKW #1
HEX_SAVE_R2		.BLKW #1
HEX_SAVE_R3		.BLKW #1
HEX_SAVE_R4		.BLKW #1
HEX_SAVE_R7		.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This is the evaluate subroutine, it does the majority of the parsing
; It first checks if the entered character is a legal operator (+ - * / ^)
; If so, it calls the OPERATOR subroutine, and then the corresponding operation subroutine
; It then pushes the result back to the stack so the next calculation can use it
; If the input is not an operator, it checks if the input is a number
; If so, the number gets pushed to the top of the stack
; If the input is neither an operator or a number, this exits and prints the invalid expression message
; R0 - character input from keyboard
; R1 - Character for comparison
; R2-R6 - Not Used
; R7 - JSR locations

EVALUATE
	ST R0, EVAL_SAVE_R0
	ST R7, EVAL_SAVE_R7	; Store the current values of R0 and R7

	LD R1, ASCII_INV_CARET
	ADD R1, R0, R1
	BRnp EVAL_PLUS		; Check if input is a ^
	JSR OPERATOR		; If so, pop two values
	JSR EXP			; Evaluate
	JSR PUSH		; Push result to stack
	BRnzp EVAL_FIN		; Go to end of subroutine
EVAL_PLUS
	LD R1, ASCII_INV_PLUS
	ADD R1, R0, R1
	BRnp EVAL_MINUS		; Check if input is a +
	JSR OPERATOR		; If so, pop two values
	JSR PLUS		; Evaluate
	JSR PUSH		; Push result to stack
	BRnzp EVAL_FIN		; Go to end of subroutine
EVAL_MINUS
	LD R1, ASCII_INV_MINUS
	ADD R1, R0, R1
	BRnp EVAL_MULT		; Check if input is a -
	JSR OPERATOR		; If so, pop two values
	JSR MIN			; Evaluate
	JSR PUSH		; Push result to stack
	BRnzp EVAL_FIN		; Go to end of subroutine
EVAL_MULT
	LD R1, ASCII_INV_MULT
	ADD R1, R0, R1
	BRnp EVAL_DIV		; Check if input is a *
	JSR OPERATOR		; If so, pop two values
	JSR MUL			; Evaluate
	JSR PUSH		; Push result to stack
	BRnzp EVAL_FIN		; Go to end of subroutine
EVAL_DIV
	LD R1, ASCII_INV_DIV
	ADD R1, R0, R1
	BRnp EVAL_NUM		; Check if input is a /
	JSR OPERATOR		; If so, pop two values
	JSR DIV			; Evaluate
	JSR PUSH		; Push result to stack
	BRnzp EVAL_FIN		; Go to end of subroutine
EVAL_NUM
	LD R1, ASCII_INV_ZERO
	ADD R0, R0, R1
	BRn EVAL_BAD_INPUT	; Check if input below ASCII numbers, if so, bad input
	ADD R1, R0, #-10
	BRp EVAL_BAD_INPUT	; Check if input above ASCII numbers, if so, bad input
	JSR PUSH		; Push input to top of stack

EVAL_FIN
	LD R0, EVAL_SAVE_R0
	LD R7, EVAL_SAVE_R7	; Restore saved registers
	RET

EVAL_BAD_INPUT
	JSR INVALID_INPUT	; If bad input, say so

ASCII_INV_CARET		.FILL	xFFA2
ASCII_INV_PLUS		.FILL 	xFFD5
ASCII_INV_MINUS		.FILL 	xFFD3
ASCII_INV_MULT		.FILL 	xFFD6
ASCII_INV_DIV		.FILL 	xFFD1
ASCII_INV_ZERO		.FILL	xFFD0

EVAL_SAVE_R0		.BLKW #1
EVAL_SAVE_R7		.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The operator subroutine prepares registers for calculations
; It pops the first value off the stack into R4, and then the next value into R3
; If there is a stack underflow at any point, exits immediately and prints the invalid expression message
; It returns the last two values from the stack in R4 and R3
; Register Table
; R0 - Output from POP subroutine
; R1 - not used
; R2 - not used
; R3 - Second value popped
; R4 - First value popper
; R5 - Holds whether the pops were successful or not
; R6 - not used
; R7 - Holds JSR and RET info

OPERATOR
	ST R0, OPR_SAVE_R0
	ST R5, OPR_SAVE_R5	
	ST R7, OPR_SAVE_R7	; Stores the registers used

	JSR POP
	ADD R5, R5, #0
	BRp OPR_INVALID		; Check for underflow
	ADD R4, R0, #0		; Move result to R4
	JSR POP
	ADD R5, R5, #0
	BRp OPR_INVALID		; Check for underflow
	ADD R3, R0, #0		; Move result to R3

	LD R0, OPR_SAVE_R0
	LD R5, OPR_SAVE_R5
	LD R7, OPR_SAVE_R7	; Restores used registers
	RET

OPR_INVALID
	JSR INVALID_INPUT	; If underflow, print Invalid Expression

OPR_SAVE_R0		.BLKW #1
OPR_SAVE_R5		.BLKW #1
OPR_SAVE_R7		.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The plus subroutine simply adds R3 and R4, and returns it in R0
; The expression is R0 = R3 + R4
; Inputs: R3, R4
; Outputs: R0
PLUS	
	ADD R0, R3, R4
RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The MIN subroutine subtracts R4 from R3 and returns it in R0
; The expression is R0 = R3 - R4
; Inputs: R3, R4
; Outputs: R0
MIN	
	ST R4, MIN_SAVE_R4	; stores original value of R4
	NOT R4, R4 
	ADD R4, R4, #1
	ADD R0, R3, R4
	LD R4, MIN_SAVE_R4	; recalls original value of R4
	RET

MIN_SAVE_R4		.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The MUL subroutine multiplies two numbers together, and returns the result in R0
; It first checks if each number is negative, and if so, adds 1 to R6 for each negative number
; It then takes the absolute value of each number, and adds |R3| to itself |R4| times and stores the result in R0
; At the very end, if the value of R6 equals 1, turn the value in R0 negative, and return R0
; The expression is R0 = R3 * R4
; 
; Register Table
; R0 - Return value
; R1 - not used
; R2 - not used
; R3 - First input
; R4 - second input, loop counter
; R5 - not used
; R6 - Holds whether result should be positive or negative
; R7 - Holds value for RET

MUL	
	ST R4, MUL_SAVE_R4	;save preexisting values of R4 and R6 to memory locations
	ST R6, MUL_SAVE_R6	;
	AND R6, R6, #0		;initialize counter of negative operands to 0
	ADD R3, R3, #0		;	take the absolute value of both operands and store it into its
	BRzp SKIPNEG 		;	original register. every time a negative operand is encountered,
	ADD R6, R6, #1		;	increment the negative operand counter
	NOT R3, R3	    	;
	ADD R3, R3, #1		;
SKIPNEG
	ADD R4, R4, #0		;
	BRzp SKIPNEG2	 	;
	ADD R6, R6, #1		;
	NOT R4, R4		;
	ADD R4, R4, #1		;
SKIPNEG2
	AND R0, R0, #0		;
	ADD R3, R3, #0		;if either operand is 0, store 0 into R0 before returning
	BRz ZERO_MULT		;
	ADD R4, R4, #0		;
	BRz ZERO_MULT		;
START	
	ADD R0, R0, R3		;add value of the first operand to the result register the value of the second 
	ADD R4, R4, #-1		;	operand number of times
	BRp START	    	;
	
MUL_RET
	AND R6, R6, #1		;if the number of negative operands is 1, negate result before returning
	BRz POSITIVE_MUL	;
	NOT R0, R0	    	;if the number of negative operands is 0 or 2, return without negating
	ADD R0, R0, #1		;
POSITIVE_MUL
	LD R4, MUL_SAVE_R4	;load back values of R4 and R6 before the subroutine
	LD R6, MUL_SAVE_R6	;
	RET			;
ZERO_MULT
	AND R0, R0, #0		;if either operand is 0, set result to 0 and return
	BRnzp MUL_RET		;

MUL_SAVE_R4	.BLKW #1	;reserve memory locations for preexisting values of R4 and R6
MUL_SAVE_R6	.BLKW #1	;

	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The DIV subroutine divides one number by another and returns the quotient in R0
; This subroutine ignores any remainders as a result of the operation
; It first checks for division by zero, and prints the invalid expression message if so
; It then checks for negative numbers in the same way as the MUL subroutine
; After making both values positive, it subtracts R4 from R3 until the result is negative
; For every iteration where the result is positive, R0 is incremented by one
; The expression is R0 = R3 / R4
;
; Register Table
; R0 - Return value, the quotient
; R1 - not used
; R2 - not used
; R3 - First input, the dividend 
; R4 - Second input, the divisor
; R5 - Holds whether the result should be positive or negative
; R6 - not used
; R7 - Holds info for RET

DIV
	ADD R4, R4, #0
	BRnp DIV_CONTINUE
	JSR INVALID_INPUT	; Division by 0, exit program

DIV_CONTINUE
	ST R3, DIV_SAVE_R3
	ST R4, DIV_SAVE_R4
	ST R5, DIV_SAVE_R5	; Save registers being modified
	AND R5, R5, #0
	ADD R3, R3, #0
	BRzp DIV_CONT_ONE	; Check if R3 is negative
	ADD R5, R5, #1		; Add one to R5 if so
	NOT R3, R3
	ADD R3, R3, #1		; Make R3 positive for algorithm
DIV_CONT_ONE
	ADD R4, R4, #0
	BRzp DIV_CONT_TWO	; Check if R4 is negative
	ADD R5, R5, #1		; Add one to R5 if so
	NOT R4, R4
	ADD R4, R4, #1		; Make R4 positive for algorithm
DIV_CONT_TWO
	AND R0, R0, #0		; Initialize R0 to 0
	NOT R4, R4
	ADD R4, R4, #1		; Take 2's complement of R4
DIV_LOOP
	ADD R3, R3, R4		; Do subtraction
	BRn DIV_END			; If the result is negative, you don't have a full division
	ADD R0, R0, #1		; Add 1 to counter (quotient)
	BRnzp DIV_LOOP		; Branch back to top of division loop
DIV_END
	AND R5, R5, x01		; Check if R5 is exactly one, if so result should be negative
	BRnz DIV_CONT
	NOT R0, R0
	ADD R0, R0, #1		; Change R0 to two's complement value
DIV_CONT
	LD R3, DIV_SAVE_R3
	LD R4, DIV_SAVE_R4
	LD R5, DIV_SAVE_R5	; Load modified registers
	RET			; Return to calling routine

DIV_SAVE_R3     .BLKW #1
DIV_SAVE_R4     .BLKW #1
DIV_SAVE_R5     .BLKW #1	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The EXP subroutine raises one value to the power of another
; This does not handle negative numbers or register overflow
; It works by calling the MUL subroutine on R3 and the result of the previous MUL call, or R3 again for the first iteration
; It then decrements a loop counter, initialized to R4, until it reaches 0, then returns the final result in R0
; The expression is R0 = R3 ^ R4
;
; Register Table
; R0 - Final output, result from MUL subroutine calls
; R1 - not used
; R2 - not used
; R3 - First input, also holds previous MUL result
; R4 - Second input, input from R3 moved here for MUL calls
; R5 - Counter, initialized to R4 and decremented every MUL call
; R6 - not used
; R7 - Holds JSR and RET info
EXP
	ST R7, EXP_SAVE_R7	; store return address of EXP subroutine in a memory location
	ADD R5, R4, #0		; move second operand to R5 and set the multiplication loop counter to
	ADD R5, R5, #-1		; second operand - 1
	ADD R4, R3, #0		; set second operand of multiplication to the first operand of the EXP subroutine
START2	
	JSR MUL			; multiply a power of the first operand and first operand, storing the result in R0
	ADD R3, R0, #0		; set output of the multiplication as the first operand of the next multiplication
	ADD R5, R5, #-1		; decrement loop counter
	BRp START2		; if loop has executed < value of  second operand - 1 times, execute loop again
	ADD R0, R3, #0		; copy final result of exponentiation to R0
	LD R7, EXP_SAVE_R7	; load return address of the EXP subroutine before returning
	RET			;

EXP_SAVE_R7	
	.BLKW #1		; reserve memory location for saving return address of the EXP subroutine

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
	ST R3, POP_SaveR3	; save R3
	ST R4, POP_SaveR4	; save R3
	AND R5, R5, #0		; clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		    ;
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


; This simple subroutine prints the invalid expression message
; It then terminates the program, instead of returning
; R0 holds the starting location of the string
INVALID_INPUT
        LEA R0, INVALID_INPUT_STRING
        PUTS
        HALT
INVALID_INPUT_STRING    .STRINGZ "Invalid Expression"

        .END


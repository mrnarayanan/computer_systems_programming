; Murugan Narayanan
; MP1
; To print the ASCII characters of the hex numbers, I left shift 4 bits at a time from the number
;	to print into R3, which represents one hex digit. I then print the corresponding ASCII character
;	that represents that hex digit.	This algorithm continues until all 4 hex digits are printed. 
; To print out the histogram, I first print the corresponding character of the bin by starting with
;	the @ character and counting upwards from that ASCII code by 1. Then a space is printed. A pointer
;	points to the memory address of the first bin, and the hex number at that location is loaded into
;	R3. The ASCII-hex printing 'subroutine' is then run to print out the 4 hex digits, which is then
;	followed by a newline character. The pointer is then incremented, and in the next loop cycle the 
;	following bin's character and number is printed. This loop iterates for 27 times until all of the
;	bins of the histogram are printed. 


; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming studio, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **

.ORIG	x3000 ; starting address is x3000

;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop


; New Register table
; R0 - used for output
; R1 - digit counter
; R2 - bit counter
; R3 - holds char to print
; R4 - digit
; R5 - pointer to histogram bins
; R6 - counter to run through bins
; R7 - temporary storage

PRINT_HIST

LD R5, HIST_ADDR ; loads starting address of histogram bins
AND R6, R6, #0 ; clears bin counter
ARCH
LDR R3, R5, #0 ; loads value at bin into R3
LD R0, ATSIGN
ADD R0, R0, R6
OUT ; prints corresponding bin character
LD R0, SPACE
OUT ; prints space character
BRnzp PRINT_NUM ; runs through number printing 'subroutine'
POST
LD R0, LINE
OUT ; prints out newline character at end of line
ADD R6, R6, #1 ; increment bin counter
ADD R5, R5, #1 ; increment bin pointer
LD R7, NUM_BINS
NOT R7, R7
ADD R7, R7, #1
ADD R7, R7, R6
BRzp DONE ; checks if ran for 27 iterations
BRnzp ARCH

PRINT_NUM
AND R1, R1, #0 ; clear R1
ADD R1, R1, #4 ; initialize digit counter to 4
OUTER
ADD R1, R1, #0 ; activate digit counter to nzp
BRz FIN ; printed <4 chars?
AND R2, R2, #0 ; clear R2
AND R4, R4, #0 ; initialize digit
ADD R2, R2, #4 ; initialize bit counter to 4
INNER
ADD R2, R2, #0 ; activate bit counter into nzp
BRz PRINT ; got <4 bits from R3?
ADD R4, R4, R4 ; shift digit left
ADD R3, R3, #0 ; activate R3 into nzp
BRzp FALL
ADD R4, R4, #1 ; add 1 to digit
FALL
ADD R3, R3, R3 ; shift R3 left
ADD R2, R2, #-1 ; decrement bit counter
BRnzp INNER
PRINT
ADD R4, R4, #0 ; activate R4 into nzp
ADD R0, R4, #-9
BRnz TRUE ; is digit <= 9?
LD R0, AL
ADD R0, R0, #-10
ADD R0, R4, R0
BRnzp NEXT
TRUE
LD R0, ZERO
ADD R0, R4, R0
NEXT
OUT ; prints out a character
ADD R1, R1, #-1 ; decrement digit counter
BRnzp OUTER
FIN
BRnzp POST

DONE
HALT ; done

; the data needed by the program
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00 ; histogram starting address
STR_START	.FILL x4000	; string starting address
ATSIGN		.FILL x0040 ; ASCII @
ZERO		.FILL x0030 ; ASCII 0
AL			.FILL x0041 ; ASCII A
SPACE		.FILL x0020 ; ASCII space 
LINE		.FILL x000A ; ASCII newline

; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING	.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."

; the directive below tells the assembler that the program is done
; (so do not write any code below it!)
.END

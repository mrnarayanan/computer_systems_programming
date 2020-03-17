 ; Lab 1 
 ; assignment: develop a code to print a value stored in a register 
 ;             as a hexadecimal number to the monitor
 ; algorithm: turnin each group of four bits into a digit
 ;            calculate the corresponding ASCII character;
 ;            print the character to the monitor

; Register table
; R0 - used for output
; R1 - digit counter
; R2 - bit counter
; R3 - holds char to print
; R4 - digit

.ORIG x3000
AND R1, R1, #0 ; clear R1
ADD R1, R1, #4 ; initialize digit counter to 4
OUTER
ADD R1, R1, #0 ; activate digit counter to nzp
BRz DONE ; printed <4 chars?
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
BRnz TRUE
LD R0, ALPHA
ADD R0, R0, #-10
ADD R0, R4, R0
BRnzp NEXT
TRUE
LD R0, ZERO
ADD R0, R4, R0
NEXT
OUT
ADD R1, R1, #-1 ; decrement digit counter
BRnzp OUTER
DONE
HALT ; stop the computer

; program data section starts here
ZERO .FILL x0030
ALPHA .FILL x0041

.END


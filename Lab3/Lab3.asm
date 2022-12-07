;
; system booting code
;
            .ORIG x0200
            LD      R6, OS_SP
            
            ; Push User_PSR
            LD      R0, USER_PSR
            ADD     R6, R6, #-1
            STR     R0, R6, #0
            
            ; Push User_PC
            LD      R0, USER_PC
            ADD     R6, R6, #-1
            STR     R0, R6, #0
            
            ; Interrupt Enable
            LD      R0, KBSR_IE
            STI     R0, KBSR1 ; KBSR[14] = 1
            
            ; Init Interrupt Vector Table
            LD      R0, InterruptA
            STI     R0, INV  ; mem[x0180] = x0800
            
            RTI
            
OS_SP       .FILL   x3000
USER_PSR    .FILL   x8002 ; 1000 0000 0000 0010(User Privilege, PL0, Z)
USER_PC     .FILL   x3000

KBSR1       .FILL   xFE00 
KBSR_IE     .FILL   x4000 ; KBSR[14] = 1

InterruptA  .FILL   x0800
INV         .FILL   x0180

.END
; Keyborad Input -> Interrupt -> Find Interrupt Service Program -> mem[IntV] (Keyborad's IntV = x(01)80)
; mem[x0180] = starting address of Interrupt Service Routine = x0800
; mem[x0180] = x0800



; interrupt service routine
            .ORIG x0800
            ; Store registers
START       ST      R1, SaveR1
            ST      R2, SaveR2
            ST      R3, SaveR3
            ST      R0, SaveR0
            ST      R5, SaveR5
            
            LDI     R0, KBDR
            LD      R1, CR          ; USE R1 TO DETERMINE IF THE INPUT IS A LF
            LD      R2, ZERO
            
            ADD     R3, R4, #0      ; R3 = R4
            
            ; ------- <Enter> --------
            ADD     R1, R0, R1      ; IF = 0, THEN INPUT IS AN ENTER
            BRnp    STEP2 
            ADD     R2, R3, R2      
            BRz     SUCCESS
            ADD     R4, R4, #-1
            BR      SUCCESS
            
            
STEP2       ; ------- <0 - 9> --------
            LD      R2, ZERO
            ADD     R5, R0, #0
            ADD     R5, R5, R2
            BRn     STEP3
            
            LD      R2, NINE
            ADD     R5, R0, #0
            ADD     R5, R5, R2
            BRp     STEP3
            
            ADD     R4, R0, #0
            BR      SUCCESS
            
            
            ; ------- <char > --------
STEP3            
            ADD     R5, R0, #0
            LD      R0, ENTER1
            OUT
            ADD     R0, R5, #0
            ;;PRINT FORTY TIMES
            LD      R5, LINENUMBER
            
LOOP        OUT
            ADD     R5, R5, #-1
            BRnp    LOOP
            
            LD      R0, ENTER1
            OUT
            BR      SUCCESS
            
            
            ; Load registers
            
SUCCESS     LD      R0, SaveR0
            LD      R1, SaveR1
            LD      R2, SaveR2
            LD      R3, SaveR3
            
            RTI

SaveR0      .BLKW   1
SaveR1      .BLKW   1
SaveR2      .BLKW   1
SaveR3      .BLKW   1
SaveR5      .BLKW   1

DSR         .FILL   xFE04
DDR         .FILL   xFE06
KBSR        .FILL   xFE00
KBDR        .FILL   xFE02

CR          .FILL   xFFF6
ZERO        .FILL   xFFD0
NINE        .FILL   xFFC7
ENTER1      .FILL   x000A
SAVER00     .BLKW   1            
SAVER11     .BLKW   1
SAVER22     .BLKW   1
SAVER44     .BLKW   1
ZEROASCIIP  .FILL   x0030
NINEASCIIP  .FILL   x0039
LINENUMBER  .FILL   #40
.END

;
; user program: R4 - Counter
;
            
            .ORIG x3000
            LD      R4, SEVEN
            
L1          LD      R2, LINENUM     ; R2 = 40
PUTCHAR     ADD     R0, R4, #0
            JSR     DELAY
            OUT
            ADD     R2, R2, #-1
            BRnp    PUTCHAR
            
NEWLINE     LD      R0, ENTER
            JSR     DELAY
            OUT
            BR      L1
            
            HALT
            
DELAY       ST      R1, DELAY_R1
            LD      R1, DELAY_COUNT
DELAY_LOOP  ADD     R1, R1, #-1
            BRnp    DELAY_LOOP
            LD      R1, DELAY_R1
            RET
            
DELAY_COUNT .FILL   #256
DELAY_R1    .BLKW   #1  
SEVEN       .FILL   x0037            
LINENUM     .FILL   #40 
ENTER       .FILL   x000A
.END




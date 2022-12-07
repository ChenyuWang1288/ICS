.ORIG	x3000
        
            LD      R6, USER_SP     ; 初始化stack pointer(R6),frame pointer(control link)(R5)
            ADD     R5, R6, #0
        
            ;Push arguments;
            ; R1 = x, R2 = y;
            AND     R1, R1, #0
            AND     R2, R2, #0
            ADD     R1, R1, #0
            ADD     R2, R2, #0
            ADD     R6, R6, #-2
            STR     R1, R6, #1
            STR     R2, R6, #0
            JSR     DFS
            ADD     R0, R4, #0      ; R0 = R4, R0 = Max

            ; Find the maximum
            LD      R3, BASEN
            LDR     R1, R3, #0      ; R1 = n-1
OUTERLOOP   ADD     R1, R1, #-1
            BRn     TERMINATE
            LD      R3, BASEN
            LDR     R2, R3, #1      ; R2 = m-1

INNERLOOP   ADD     R2, R2, #-1
            BRn     OUTERLOOP
            ; Push arguments
            ADD     R6, R6, #-2
            STR     R1, R6, #1
            STR     R2, R6, #0
            ST      R0, SaveR0
            ST      R1, SaveR11
            ST      R2, SaveR22
            JSR     DFS
            LD      R0, SaveR0
            LD      R1, SaveR11
            LD      R2, SaveR22
            ADD     R3, R4, #0      ; R3 = R4
            NOT     R4, R4
            ADD     R4, R4, #1
            ADD     R4, R0, R4      ; R4 = Max-tempMax
            BRzp    INNERLOOP
            ADD     R0, R3, #0      ; Max = tempMax
            BR      INNERLOOP


TERMINATE   ADD     R2, R0, #0
            HALT     
; SUBROUTINE DFS
DFS         ADD     R6, R6, #-1     ; PUSH CONTROL LINK
            STR     R5, R6, #0
            ADD     R5, R6, #0
            
            ADD     R6, R6, #-1     ; PUSH RETURN ADDRESS
            STR     R7, R6, #0
            
            AND     R3, R3, #0      ; INTITIALIZE THE TEMPORARY 
            AND     R4, R4, #0
            ADD     R4, R4, #1
            
            ADD     R6, R6, #-1
            STR     R3, R6, #0
            ADD     R6, R6, #-1
            STR     R4, R6, #0
            
            ; judge the condition to END THE RECCURENCE
            LDR     R2, R5, #1      ; R2 = y, R1 = x
            LDR     R1, R5, #2
            
            ADD     R1, R1, #0      ; If x<0
            BRn     RETURN0
            
            LD      R3, BASEN
            LDR     R4, R3, #0      ; R4 = n
            NOT     R1, R1
            ADD     R1, R1, #1
            ADD     R1, R4, R1
            BRnz    RETURN0         ; If x>=n
            
            ADD     R2, R2, #0
            BRn     RETURN0         ; If y<0
            
            LD      R3, BASEN
            LDR     R4, R3, #1      ; R4 = M
            NOT     R2, R2
            ADD     R2, R2, #1
            ADD     R2, R2, R4
            BRnz    RETURN0         ; If y>=n
            
            ; Go Up: if(a[x][y] > a[x-1][y])
NEXT1       LDR     R2, R5, #1      ; R2 = y, R1 = x
            LDR     R1, R5, #2
            ADD     R1, R1, #0      
            BRz     NEXT2
            LD      R3, BASEN       ; R3 = x4000
            LDR     R4, R3, #1      ; R4 = m
            JSR     MULTIPLY
            ADD     R0, R2, R0
            ADD     R0, R0, #2      ; R0 = x*m+y+2
            ADD     R3, R3, R0
            LDR     R2, R3, #0      ; R2 = a[x*m+y+2]
            NOT     R4, R4
            ADD     R4, R4, #1
            ADD     R3, R3, R4
            LDR     R1, R3, #0      ; R1 = a[(x-1)*m+y+2]
            NOT     R1, R1
            ADD     R1, R1, #1
            ADD     R1, R2, R1
            BRnz    NEXT2
            
            LDR     R2, R5, #1      ; R2 = y, R1 = x-1
            LDR     R1, R5, #2
            ADD     R1, R1, #-1
            ADD     R6, R6, #-2     ; Push arguments
            STR     R1, R6, #1
            STR     R2, R6, #0
            JSR     DFS
            
            LDR     R3, R5, #-3     ; R3 = res, R4 = temp
            ADD     R4, R4, #1      
            ADD     R0, R4, #0      ; R0 = R4
            NOT     R4, R4
            ADD     R4, R4, #1
            ADD     R4, R3, R4      ; R4 = res-temp
            BRzp    NEXT2
            ADD     R3, R0, #0      ; res = temp
            ADD     R4, R3, #0
            STR     R4, R5, #-3


            ; Go Down: if(a[x][y] > a[x+1][y])            
NEXT2       LDR     R2, R5, #1      ; R2 = y, R1 = x
            LDR     R1, R5, #2
            LD      R3, BASEN       
            LDR     R4, R3, #0      ; R4 = n-1
            ADD     R4, R4, #-1
            NOT     R4, R4
            ADD     R4, R4, #1
            ADD     R4, R1, R4
            BRz     NEXT3
            LD      R3, BASEN       ; R3 = x4000
            LDR     R4, R3, #1      ; R4 = m
            JSR     MULTIPLY
            ADD     R0, R2, R0
            ADD     R0, R0, #2      ; R0 = x*m+y+2
            ADD     R3, R3, R0
            LDR     R2, R3, #0      ; R2 = a[x*m+y+2]
            ADD     R3, R3, R4
            LDR     R1, R3, #0      ; R1 = a[(x+1)*m+y+2]
            NOT     R1, R1
            ADD     R1, R1, #1
            ADD     R1, R2, R1
            BRnz    NEXT3
            
            LDR     R2, R5, #1      ; R2 = y, R1 = x+1
            LDR     R1, R5, #2
            ADD     R1, R1, #1
            ADD     R6, R6, #-2     ; Push arguments
            STR     R1, R6, #1
            STR     R2, R6, #0
            JSR     DFS
            
            LDR     R3, R5, #-3     ; R3 = res, R4 = temp
            ADD     R4, R4, #1      
            ADD     R0, R4, #0      ; R0 = R4
            NOT     R4, R4
            ADD     R4, R4, #1
            ADD     R4, R3, R4      ; R4 = res-temp
            BRzp    NEXT3
            ADD     R3, R0, #0      ; res = temp
            ADD     R4, R3, #0
            STR     R4, R5, #-3
            
            ; Go Left: if(a[x][y] > a[x][y-1])
NEXT3       LDR     R2, R5, #1      ; R2 = y, R1 = x
            LDR     R1, R5, #2
            ADD     R2, R2, #0
            BRz     NEXT4
            LD      R3, BASEN       ; R3 = x4000
            LDR     R4, R3, #1      ; R4 = m
            JSR     MULTIPLY
            ADD     R0, R2, R0
            ADD     R0, R0, #2      ; R0 = x*m+y+2
            ADD     R3, R3, R0
            LDR     R2, R3, #0      ; R2 = a[x*m+y+2]
            ADD     R3, R3, #-1
            LDR     R1, R3, #0      ; R1 = a[x*m+y-1+2]
            NOT     R1, R1
            ADD     R1, R1, #1
            ADD     R1, R2, R1
            BRnz    NEXT4
            
            LDR     R2, R5, #1      ; R2 = y-1, R1 = x
            LDR     R1, R5, #2
            ADD     R2, R2, #-1
            ADD     R6, R6, #-2     ; Push arguments
            STR     R1, R6, #1
            STR     R2, R6, #0
            JSR     DFS
            
            LDR     R3, R5, #-3     ; R3 = res, R4 = temp
            ADD     R4, R4, #1      
            ADD     R0, R4, #0      ; R0 = R4
            NOT     R4, R4
            ADD     R4, R4, #1
            ADD     R4, R3, R4      ; R4 = res-temp
            BRzp    NEXT4
            ADD     R3, R0, #0      ; res = temp
            ADD     R4, R3, #0      ; R4 = res
            STR     R4, R5, #-3
            
            ; Go Right: if(a[x][y] > a[x][y+1])
NEXT4       LDR     R2, R5, #1      ; R2 = y, R1 = x
            LDR     R1, R5, #2
            LD      R3, BASEN       ; R3 = x4000
            LDR     R4, R3, #1
            ADD     R4, R4, #-1
            NOT     R4, R4
            ADD     R4, R4, #1
            ADD     R4, R4, R2
            BRz     RETURNRES
            LDR     R4, R3, #1      ; R4 = m
            JSR     MULTIPLY
            ADD     R0, R2, R0
            ADD     R0, R0, #2      ; R0 = x*m+y+2
            ADD     R3, R3, R0
            LDR     R2, R3, #0      ; R2 = a[x*m+y+2]
            ADD     R3, R3, #1
            LDR     R1, R3, #0      ; R1 = a[x*m+y+1+2]
            NOT     R1, R1
            ADD     R1, R1, #1
            ADD     R1, R2, R1
            BRnz    RETURNRES
            
            LDR     R2, R5, #1      ; R2 = y+1, R1 = x
            LDR     R1, R5, #2
            ADD     R2, R2, #1
            ADD     R6, R6, #-2     ; Push arguments
            STR     R1, R6, #1
            STR     R2, R6, #0
            JSR     DFS
            
            LDR     R3, R5, #-3     ; R3 = res, R4 = temp
            ADD     R4, R4, #1      
            ADD     R0, R4, #0      ; R0 = R4
            NOT     R4, R4
            ADD     R4, R4, #1
            ADD     R4, R3, R4      ; R4 = res-temp
            BRzp    RETURNRES
            ADD     R3, R0, #0      ; res = temp
            ADD     R4, R3, #0
            STR     R4, R5, #-3
            
            
            
            BR      RETURNRES
            
RETURN0     LDR     R7, R5, #-1
            LDR     R5, R5, #0
            ADD     R6, R6, #6
            AND     R4, R4, #0
            BR      RETURN
            
RETURNRES   LDR     R7, R5, #-1
            LDR     R4, R5, #-3
            LDR     R5, R5, #0
            ADD     R6, R6, #6
                 
RETURN      RET

; This subroutine multiply values in R1,R4, return result in R0
; R1 = x, R4 = m, R0 = x*m
MULTIPLY    ST      R1, SaveR1
            ST      R4, SaveR4
            
            AND     R0, R0, #0
LOOP        ADD     R4, R4, #0
            BRz     ENDMUL
            ADD     R4, R4, #-1
            ADD     R0, R1, R0
            BR      LOOP
            
ENDMUL      LD      R1, SaveR1
            LD      R4, SaveR4

            RET
;;
BASEN       .FILL   x4000
USER_SP     .FILL   xFE00
; the arguments in subroutine MULTIPLY
SaveR1      .BLKW   1
SaveR4      .BLKW   1
SaveR0      .BLKW   1
SaveR11     .BLKW   1
SaveR22     .BLKW   1
.END



NAME ASM_FUNCTIONS


; _2cycle(1)-> move r7, 1, lcall _2cycle, total 3cycle
; so _2cycle(n)=> 2n+5 cycles, n from 1...255;

PUBLIC __2cycle
_ASMCOD SEGMENT CODE
RSEG _ASMCOD
__2cycle:
	      DJNZ r7,__2cycle ;2 Cycle, first char Arg in r7
		  RET              ;2 Cycle
END



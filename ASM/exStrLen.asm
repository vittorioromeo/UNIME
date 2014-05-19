%include "commonMacros.asm"
%include "commonFuncs.asm"

global _start 

; SECTION: initialized data
section .data							
	strBuffer: times 100 db 0
	strStar db '*'
	strTest db 'ahahaha', 0

; SECTION: uninitialized data
section .bss					
	len resb 1

; SECTION: code
section .text
	_start:

 		push strTest
 		push 9
		call ssvcf_linuxPrintTerm
		MC_exitProgram 0

		MC_linuxTermRead strBuffer, 100
		;MC_linuxTermPrint strBuffer, 100

		push eax
		push ebx

		push strBuffer
		call fnStrLen
		mov [len], eax
		
		pop ebx
		pop eax

		mov eax, [len]
	
		printLoop:
			cmp eax, 0
			jz printLoopEnd
			MC_linuxTermPrint strStar, 1
			dec eax
			jmp printLoop

		printLoopEnd:
			MC_exitProgram 0

	fnStrLen:
		mov eax, [esp + 4]
		mov ebx, eax

		loopCount:
			cmp byte [eax], 0
			jz loopEnd
			inc eax
			jmp loopCount

		loopEnd:
			sub eax, ebx
			ret
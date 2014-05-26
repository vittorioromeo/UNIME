%include "commonMacros.asm"

; MACRO: read an operand from terminal and put it in a variable
;	%1: target byte
%macro MC_E_readOperand 1
	; Read value
	MC_linuxTermReadWithLen strBuffer, strBufferLen, [lastReadLen]	
	
	; Save eax
	push eax
	
	; Pass parameters to fnConvertStrToNum
	push strBuffer
	push lastReadLen

	; fnConvertStrToNum returns its value in eax
	call fnConvertStrToNum
	mov %1, eax

	; Discard parameters
	pop edx
	pop edx

	; Restore eax
	pop eax
%endmacro

; MACRO: easy-to-use division
;	%1: operand 1
;	%2: operand 2
; 	%3: target quotient byte
; 	%4: target remainder byte
%macro MC_E_division 4
	; Save registers
	push eax
	push ebx
	push ecx
	push edx

	; Perform division
	mov edx, 0 		; clear dividend
	mov eax, %1   	; dividend
	mov ecx, %2    	; divisor
	idiv ecx           

	; Move results in target bytes
	mov %3, eax
	mov %4, edx

	; Restore registers
	pop edx
	pop ecx
	pop ebx
	pop eax
%endmacro

global _start 

; SECTION: initialized data
section .data
	strMsg0: db "Insert multiply operand 1", 0Ah, 0							
	strMsg0Len: equ $-strMsg0
	strMsg1: db "Insert multiply operand 2", 0Ah, 0
	strMsg1Len: equ $-strMsg1
	strMsg2: db "Insert division operand 1", 0Ah, 0
	strMsg2Len: equ $-strMsg2
	strMsg3: db "Insert division operand 2", 0Ah, 0
	strMsg3Len: equ $-strMsg3

	strBuffer: times 100 db 0
	strBufferLen: equ $-strBuffer
	
	strStar: db "*", 0
	strStarLen: equ $-strStar

; SECTION: uninitialized data
section .bss
	lastReadLen: resb 4		
	operand0: resb 4			
	operand1: resb 4
	operationResult: resb 4
	
	divResult: resb 4
	divRemainder: resb 4

; SECTION: code
section .text
	_start:

		; MULTIPLICATION
		MC_linuxTermPrint strMsg0, strMsg0Len
		MC_E_readOperand [operand0]

		MC_linuxTermPrint strMsg1, strMsg1Len
		MC_E_readOperand [operand1]

		mov eax, [operand0]
		imul eax, [operand1]
		push eax
		push strBuffer
		call fnConvertNumToStr
		MC_linuxTermPrint strBuffer, 10


		; DIVISION
		MC_linuxTermPrint strMsg2, strMsg2Len
		MC_E_readOperand [operand0]

		MC_linuxTermPrint strMsg3, strMsg3Len
		MC_E_readOperand [operand1]

		MC_E_division [operand0], [operand1], [divResult], [divRemainder]
		mov eax, [divResult]
		push eax
		push strBuffer
		call fnConvertNumToStr
		MC_linuxTermPrint strBuffer, 10



		MC_exitProgram 0




	; FUNCTION: convert string to integer
	;	stack param 1: source string buffer
	;	stack param 2: source string buffer length
	;	returns the converted value in eax
	fnConvertStrToNum:
		; Save registers
		push ebx
		push ecx
		push edx

		; Get user-passed parameters
		mov eax, [esp + 12 + 8]	; buffer ptr
		mov ebx, [esp + 12 + 4]	; buffer len
		mov ebx, [ebx]
		
		; Initialize temp variables
		mov ecx, 0  			; result
		;mov edx, 0 				; temp

		loopCount:
			; if(ebx == 0) goto loopEnd;
			cmp ebx, 0
			je loopEnd	
			
			; edx[0..4] = 0
			; edx[0] = eax[0]
			mov edx, 0 
			movzx edx, byte [eax]
			
			; Check if edx contains a valid char digit
			cmp edx, '0'
			jl loopEnd
			cmp edx, '9'
			jg loopEnd

			; Convert char digit to number
			sub edx, '0'
			imul ecx, 10
			add ecx, edx
	
			; --ebx;
			; ++eax;		
			dec ebx
			inc eax
			jmp loopCount

		loopEnd:
			; Store return result in eax
			mov eax, ecx

			; Restore registers
			pop edx
			pop ecx
			pop ebx

			; Return
			ret




	; FUNCTION: convert integer to string
	;	stack param 1: number to convert
	;	stack param 2: target output string buffer
	;   stack param TODO: number of digit chars 
	fnConvertNumToStr:
		; Save registers to the stack
		push eax
		push ebx
		push edx
		
		; Get user-passed parameters
		mov eax, [esp + 12 + 8]	; number
		mov ebx, [esp + 12 + 4]	; output buffer

		; Initialize temp variables
		mov edx, 0 				; number of digits

		; Find out number of digits
		push eax
		loopDigitCount:
			; if(eax == 0) goto loopDigitCountEnd;
			cmp eax, 0
			je loopDigitCountEnd
			
			; eax /= 10;
			MC_E_division eax, 10, [divResult], [divRemainder]
			mov eax, [divResult]
			
			; ++edx;
			inc edx

			jmp loopDigitCount

		loopDigitCountEnd:
			pop eax

		; eax == number
		; edx == digits

		loopCount_2:
			; if(eax == 0) goto loopEnd_2;
			cmp eax, 0
			je loopEnd_2
			
			; eax /= 10;
			MC_E_division eax, 10, [divResult], [divRemainder]
			mov eax, [divResult]

			; convert digit to char
			mov ch, [divRemainder]
			add ch, '0'

			; Set char digit (starting from the end) of the target
			; output buffer
			dec edx
			mov [ebx + edx], ch

			jmp loopCount_2

		loopEnd_2:
			; Restore registers
			pop edx			
			pop ebx
			pop eax

			; Return 
			ret
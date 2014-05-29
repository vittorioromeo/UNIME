%include "commonMacros.asm"

; cdecl:
; *) arguments are passed on the stack
; *) ints and pointers are returned in eax
; *) eax, ecx and edx are caller-saved
; *) other registers are callee-saved
; *) caller cleans stack

; MACRO: read an operand from terminal and put it in a variable
;	%1: target byte
%macro MC_E_readOperand 1
	; Read value
	MC_linuxTermReadWithLen strBuffer, strBufferLen, [lastReadLen]	
	
	; Save registers
	push eax
	push ecx
	push edx
	
	push strBuffer
	push lastReadLen
	call fnConvertStrToNum
	mov %1, eax
	add esp, 8
	
	; Restore registers
	pop edx
	pop eax
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
	push ecx
	push edx

	; Perform division
	mov edx, 0 		; clear dividend register
	mov eax, %1   	; dividend
	mov ecx, %2    	; divisor
	idiv ecx           

	; Move results in target bytes
	mov %3, eax
	mov %4, edx

	; Restore registers
	pop edx
	pop ecx
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

		; No need to save registers
		
		mov eax, [operand0]
		imul eax, [operand1]
		
		push eax
		push strBuffer
		call fnConvertNumToStr
		MC_linuxTermPrint strBuffer, 10
		add esp, 8

		; DIVISION
		MC_linuxTermPrint strMsg2, strMsg2Len
		MC_E_readOperand [operand0]

		MC_linuxTermPrint strMsg3, strMsg3Len
		MC_E_readOperand [operand1]

		; No need to save registers

		mov eax, [operand0]
		push eax
		mov eax, [operand1]
		push eax
		push divRemainder
		call fnIntDivision
		add esp, 12

		; No need to save registers

		push eax
		push strBuffer
		call fnConvertNumToStr
		MC_linuxTermPrint strBuffer, 10
		add esp, 8

		MC_exitProgram 0



	; FUNCTION: convert string to integer
	;	stack param 1: source string buffer
	;	stack param 2: source string buffer length
	;	returns the converted value in eax
	fnConvertStrToNum:
		push ebx				; Save registers
		
		mov eax, [esp + 4 + 8]	; buffer ptr
		mov ebx, [esp + 4 + 4]	; buffer len
		mov ebx, [ebx]
		
		mov ecx, 0  			; result

		.loop:
			; if(ebx == 0) goto .loopEnd;
			cmp ebx, 0
			je .loopEnd	
			
			; edx[0..4] = 0;
			; edx[0] = eax[0];
			mov edx, 0 
			movzx edx, byte [eax]
			
			; Check if edx contains a valid char digit
			cmp edx, '0'
			jl .loopEnd
			cmp edx, '9'
			jg .loopEnd

			; Convert char digit to number
			sub edx, '0'
			imul ecx, 10
			add ecx, edx
			
			dec ebx
			inc eax
			jmp .loop

		.loopEnd:
			mov eax, ecx	; Store return result in eax			
			pop ebx			; Restore registers							
			ret



	; FUNCTION: convert integer to string
	;	stack param 1: number to convert
	;	stack param 2: target output string buffer
	;   stack param TODO: number of digit chars 
	fnConvertNumToStr:
		; Save registers to the stack
		push ebx
		
		; Get user-passed parameters
		mov eax, [esp + 4 + 8]	; number
		mov ebx, [esp + 4 + 4]	; output buffer

		; Initialize temp variables
		mov edx, 0 				; number of digits

		; Find out number of digits
		push eax
		.loopCountDigit:
			; if(eax == 0) goto .loopCountDigitEnd;
			cmp eax, 0
			je .loopCountDigitEnd
			
			; eax /= 10;
			MC_E_division eax, 10, [divResult], [divRemainder]
			mov eax, [divResult]
			
			; ++edx;
			inc edx

			jmp .loopCountDigit

		.loopCountDigitEnd:
			pop eax

		; eax == number
		; edx == digits

		.loop:
			; if(eax == 0) goto .loopEnd;
			cmp eax, 0
			je .loopEnd
			
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

			jmp .loop

		.loopEnd:
			; Restore registers
			pop ebx			

			; Return 
			ret


	; FUNCTION:
	;	stack param 1: dividend
	;	stack param 2: divisor
	;	stack param 3: remainder target byte
	; returns quotient in eax, sets remainder target byte
	fnIntDivision:
		push ebx	; Save registers

		mov edx, 0 		; clear dividend register
		mov eax, [esp + 4 + 12]	; dividend
		mov ecx, [esp + 4 + 8]	; divisor		
		idiv ecx				; Perform division

		; Move results in target bytes
		mov ebx, [esp + 4 + 4]
		mov [ebx], edx

		pop ebx		; Restore registers		
		ret
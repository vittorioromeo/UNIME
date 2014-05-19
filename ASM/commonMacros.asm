%ifndef SSV_COMMON_MACROS
%define SSV_COMMON_MACROS

; MACRO: linux print implementation
%macro MC_linuxPrintImpl 3
	push ebx
	push eax
	push ecx
	push edx 

	mov ebx, %1  		; | <- print target (1=stdout)
	mov eax, 4	  		; Print 100 bytes starting from str
	mov ecx, %2			; | <- source buffer
	mov edx, %3			; | <- number of characters to print
	int 80h				; \___

	pop edx
	pop ecx
	pop eax
	pop ebx
%endmacro

; MACRO: read from terminal
%macro MC_linuxTermRead 2
	push eax
	push ebx
	push ecx
	push edx

	mov eax, 3			; Read user input into str 
	mov ebx, 0 			; | <- read source (0=stdin) 
	mov ecx, %1			; | <- destination buffer 
	mov edx, %2			; | <- max buffer length
	int 80h				; \___

	pop edx
	pop ecx
	pop ebx
	pop eax
%endmacro

; MACRO: get read string's length and put it in %3
%macro MC_linuxTermReadWithLen 3
	push eax
	push ebx
	push ecx
	push edx

	mov eax, 3			; Read user input into str 
	mov ebx, 0 			; | <- read source (0=stdin) 
	mov ecx, %1			; | <- destination buffer 
	mov edx, %2			; | <- max buffer length
	int 80h				; \___

	mov %3, eax
	pop edx
	pop ecx
	pop ebx
	pop eax
%endmacro

; MACRO: print to terminal
%macro MC_linuxTermPrint 2
	MC_linuxPrintImpl 1, %1, %2	
%endmacro

; MACRO: print to file
%macro MC_linuxFileWrite 2
 	MC_linuxPrintImpl eax, %1, %2	
%endmacro

; MACRO: exit program
%macro MC_exitProgram 1
	mov eax, 1   		; Return
	mov ebx, %1      	; | <- return code
	int 80h         	; \___
%endmacro

; MACRO: create a file
%macro MC_linuxFileCreate 2
	push eax
	push ebx
	push ecx

	mov eax, 8			; Create file
	mov ebx, %1			; | <- file path	
	mov ecx, %2			; | <- permissions
	int 80h 			; \___

	pop ecx
	pop ebx
	pop eax
%endmacro

; MACRO: open a file
%macro MC_linuxFileOpen 3
	push eax
	push ebx
	push ecx
	push edx

	mov eax, 5			; Open file
	mov ebx, %1			; | <- file path
	mov ecx, %2 		; | <- method (0=RDONLY, 1=WRONLY, 2=RDWR, ...)
	mov edx, %3			; | <- permissions
	int 80h 			; \___

	pop edx
	pop ecx
	pop ebx
	pop eax
%endmacro

; MACRO: close last opened file
%macro MC_linuxFileClose 0
	mov eax, 6			; Close file
	int 80h 			; \___
%endmacro

%endif
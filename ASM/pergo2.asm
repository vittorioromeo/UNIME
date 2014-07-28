section .data
	strMsg0: db "Il carattere inserito non è numerico", 0Ah, 0							
	strMsg0Len: equ $-strMsg0

section .bss
	numeroInserito: resb 1
	accumulatore: resb 1

global _start

section .text
_start:
	; leggo carattere da tastiera
	mov eax, 3 ; servizio 3
	mov ebx, 0 ; legge da terminale
	mov ecx, numeroInserito ; puntatore stringa bersaglio
	mov edx, 1 ; num max caratteri
	int 80h ; chiamo il servizio

	; verifico che il carattere sia un numero
	mov dh, [numeroInserito]
	cmp dh, '0'
	jl nonNumero

	cmp dh, '9'
	jg nonNumero

	jmp numero

	nonNumero:
		; stampa delle stringhe
		mov eax, 4 ; servizio 4
		mov ebx, 0 ; stampa a terminale
		mov ecx, strMsg0
		mov edx, strMsg0Len
		int 80h

		; uscita dal programma
		mov eax, 1 ; servizio 1
		mov ebx, 1 ; errore
		int 80h

	numero:
		mov byte [accumulatore], 0
		mov ecx, 0 ; variabile contatore

		; converto il carattere in numero
		sub dh, '0'

		ciclo:
			add [accumulatore], dh
			inc dh

			inc ecx
			cmp ecx, 5
			jl ciclo

	; uscita dal programma
	mov eax, 1 ; servizio 1
	mov ebx, [accumulatore] ; valore accumulato
	int 80h
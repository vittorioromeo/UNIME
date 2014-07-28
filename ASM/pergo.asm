section .data
	strMsg0: db "Il carattere inserito non è alfabetico", 0Ah, 0							
	strMsg0Len: equ $-strMsg0

section .bss
	carattereInserito: resb 1
	strMinuscola: resb 5
	strMaiuscola: resb 5

global _start

section .text
_start:
	; leggo carattere da tastiera
	mov eax, 3 ; servizio 3
	mov ebx, 0 ; legge da terminale
	mov ecx, carattereInserito ; puntatore stringa bersaglio
	mov edx, 1 ; num max caratteri
	int 80h ; chiamo il servizio

	; è un carattere alfabetico?

	mov dh, [carattereInserito] ; contiene il carattere

	cmp dh, 'A'
	jl nonAlfabetico

	cmp dh, 'Z'
	jle alfabetico

	cmp dh, 'a'
	jge parte2
	jmp nonAlfabetico

	parte2:
		cmp dh, 'z'
		jle alfabetico

	nonAlfabetico: 
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

	alfabetico:	
		cmp dh, 'A'
		jge parte2maiuscolo

		parte2maiuscolo:
			cmp dh, 'Z'
			jle maiuscolo
		
		jmp minuscolo

		maiuscolo:
			add dh, 20h

		minuscolo:
			mov ebx, 0 ; contatore caratteri

			ciclo:
				mov ch, dh ; carattere da mettere nelle stringhe
				mov [strMinuscola + ebx], ch
				sub ch, 20h ; trasformiamo il carattere in ecx in maiuscolo
				mov [strMaiuscola + ebx], ch		

				inc ebx
				inc dh

				cmp ebx, 5 ; se ebx < 5
				jl ciclo

	; stampa delle stringhe
	mov eax, 4 ; servizio 4
	mov ebx, 0 ; stampa a terminale
	mov ecx, strMinuscola
	mov edx, 5
	int 80h

	mov eax, 4 ; servizio 4
	mov ebx, 0 ; stampa a terminale
	mov ecx, strMaiuscola
	mov edx, 5
	int 80h

	; uscita dal programma
	mov eax, 1 ; servizio 1
	mov ebx, 0 ; tutto ok
	int 80h
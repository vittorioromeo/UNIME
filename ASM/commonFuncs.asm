%ifndef SSV_COMMON_FUNCS
%define SSV_COMMON_FUNCS

ssvcf_impl_saveRegisters:
	push eax
	push ebx
	push ecx
	push edx
	ret

ssvcf_impl_restoreRegisters:
	pop edx
	pop ecx
	pop ebx
	pop eax
	ret

ssvcf_linuxPrintTerm:
	
	push ebx
	push eax
	push ecx
	push edx 

	mov ebx, 1  		
	mov eax, 4	  		
	mov ecx, [esp + 8 + 16]	
	mov edx, [esp + 4 + 16]
	int 80h				

	pop edx
	pop ecx
	pop eax
	pop ebx

	ret

%endif
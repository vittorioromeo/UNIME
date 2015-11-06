; Explicit subtraction
t0(): 
	subq	$24, %rsp
	xorl	%eax, %eax
	movl	$.LC0, %edi
	leaq	8(%rsp), %rdx
	leaq	12(%rsp), %rsi
	call	scanf
	movl	12(%rsp), %esi
	movl	8(%rsp), %ecx
	movl	%esi, %eax
	cltd
	idivl	%ecx
	imull	%eax, %ecx
	cmpl	%esi, %ecx
	sete	%al
	movb	%al, 7(%rsp)
	addq	$24, %rsp
	ret

; No explicit subtraction
t1():
	subq	$24, %rsp
	xorl	%eax, %eax
	movl	$.LC0, %edi
	leaq	12(%rsp), %rdx
	leaq	8(%rsp), %rsi
	call	scanf
	movl	8(%rsp), %esi
	movl	12(%rsp), %ecx
	movl	%esi, %eax
	cltd
	idivl	%ecx
	imull	%eax, %ecx
	cmpl	%esi, %ecx
	sete	%al
	movb	%al, 7(%rsp)
	addq	$24, %rsp
	ret
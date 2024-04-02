	.text
	.globl puts
// int puts(const char *s);
puts:
	push	%rdi
	push 	%rsi
	push 	%rdx
	push 	%rbx
	push	%rax
	
	mov	%rdi,%rsi # *s is in %rdi
	xor	%rdx,%rdx # %rdx = 0
	
// compute the string length	
_sl:	
	mov	(%rdi),%bl
	cmp	$0,%bl
	je	_el
	inc	%rdi
	inc	%rdx
	jmp	_sl
	
_el:	cmp	$0,%rdx
	je	_ef

// invoke the write system call	
	mov	$1,%rax
	mov	$1,%rdi
	syscall
	
_ef:
	pop	%rax
	pop	%rbx
	pop	%rdx
	pop	%rsi
	pop	%rdi
	ret

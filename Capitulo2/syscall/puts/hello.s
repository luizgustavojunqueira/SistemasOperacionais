	.section        .rodata
msg:    .ascii "Hello World!\n"
	.quad	0

	.text
	.globl _start

_start:
	movq	$msg,%rdi	# $msg is the address of the string
	call	puts

	movq	$0,%rdi
	call	exit

	

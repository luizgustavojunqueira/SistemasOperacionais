	.section        .rodata
msg:    .ascii "Hello World!\n"
	.quad	0

	.text
	.globl _start

_start:
	movq	$1,%rax		# $1 is the number for the write syscall
	movq	$1,%rdi		# $1 represents the stdout
	movq	$msg,%rsi	# $msg is the address of the string
	movq	$13,%rdx	# $13 is the lenght of the string
	syscall			# make the system call

	movq 	$60,%rax	# $60 is the number for the exit syscall
	movq 	$0,%rdi		# $0 is the return value of exit: exit(0)
	syscall			# make the system call

	

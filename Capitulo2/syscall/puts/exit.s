	.text
	.globl	exit
exit:	
	movq 	$60,%rax	# $60 is the number for the exit syscall
	movq 	$0,%rdi		# $0 is the return value of exit: exit(0)
	syscall			# make the system call

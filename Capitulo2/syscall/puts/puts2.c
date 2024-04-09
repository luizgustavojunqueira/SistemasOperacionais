static int puts(const char *s)
{
	long len = 0;
	char *t = (char *)s;

	while (*t++) len++;
	if (len) {
		asm("movq %0,%%rsi" : : "a" (s));
		asm("movq %0,%%rdx" : : "a" (len));
		asm("movq $1,%rax");
		asm("movq $1,%rdi");
		asm("syscall");
	}
	return len;
}

static void exit(void)
{
	asm("movq $60,%rax");
	asm("movq $0,%rdi");
	asm("syscall");
}

int _start()
{
	puts("Hello world!\n");
	exit();
}

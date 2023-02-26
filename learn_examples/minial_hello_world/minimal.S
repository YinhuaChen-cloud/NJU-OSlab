#include <sys/syscall.h>

.global _start
_start:
	movq $SYS_write, %rax
	movq $1,         %rdi
	movq $st,        %rsi
	movq $(ed - st), %rdx
	syscall

	movq $SYS_exit,  %rax
	movq $0,         %rdi
	syscall

st:
	.ascii "Hello, OS World\n"
ed:

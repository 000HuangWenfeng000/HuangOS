
.global load_tr
.global farjmp
.global farcall

.section .text


//void load_tr(int tr)
load_tr:
	ltr 4(%esp)
	ret

// void farjmp(int offset, int selector)
farjmp:
	mov %esp, %eax
	add $4, %eax
	ljmp *(%eax)
	ret

// void farcall(int offset, int selector)
farcall:
	mov %esp, %eax
	add $4, %eax
	lcall *(%eax)
	ret

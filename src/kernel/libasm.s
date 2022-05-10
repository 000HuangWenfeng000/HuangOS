

.section .text

.global io_in8
.global io_out8
.global load_eflags
.global store_eflags

.global sti;
.global cli;

.global hlt
.global stihlt


// void io_in
io_in8:
	push %ebp
	mov %esp, %ebp

	mov 8(%ebp), %edx
	mov $0, %eax
	in %dx, %al

	leave
	ret

// void io_out8(int port, int data);
io_out8:
	push %ebp
	mov %esp, %ebp 
	
	mov 8(%ebp), %edx
	mov 12(%ebp), %eax
	out %al, %dx

	leave
	ret

// void io_hlt();
hlt:
	hlt
	ret

// void io_stihlt();
stihlt:
	sti 
	hlt 
	ret

// void cli();
cli:
	cli
	ret 

// void sti();	
sti:
	sti 
	ret

// int load_eflags();
load_eflags:
	pushf
	pop %eax
	ret 

// void store_eflags(int eflags);
store_eflags:
	push %ebp
	mov %esp, %ebp
	mov 8(%ebp), %eax
	push %eax 
	popf
	leave
	ret

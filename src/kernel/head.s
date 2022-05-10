/*******************************************************************
*	file : head.s
*
*	
*	kernel header file
*
*	author : HuangWF
* 
*
********************************************************************/

.section .text
.global _start 

_start:
	lgdt GDT_POINTER
	lidt IDT_POINTER

	mov $0x10, %ax 
	mov %ax, %ds 
	mov %ax, %es 
	mov %ax, %fs 
	mov %ax, %gs 
	mov %ax, %ss 
	movl $0x7e00, %esp
	
go_to_kernel:
	jmp KernelMain 


.section .data 
.global GDT_BASE
.global IDT_BASE

// GDT
.align 8
.equ GDT_ENTRIES, 8192
GDT_TABLE:			
	.word 0, 0, 0, 0
	.word 0xFFFF, 0x0000, 0x9A00, 0x00CF
	.word 0xFFFF, 0x0000, 0x9200, 0x00CF
	.fill GDT_ENTRIES - 3, 8, 0
GDT_END:
GDT_POINTER: 			
GDT_LIMIT:	.word GDT_ENTRIES * 8 - 1
GDT_BASE:	.int GDT_TABLE

// IDT
.align 8
.equ IDT_ENTRIES, 256
IDT_TABLE:
	.fill IDT_ENTRIES, 8, 0
IDT_END:
IDT_POINTER:			
IDT_LIMIT:	.word IDT_ENTRIES * 8 - 1
IDT_BASE:	.int IDT_TABLE

// init page


/*******************************************************************
*	file : interrupt.s
*
*	
*	kernel header file
*
*	author : HuangWF
* 
*
********************************************************************/

.global interrupt_handler00
.global interrupt_handler01
.global interrupt_handler06
.global interrupt_handler08
.global interrupt_handler0d
.global interrupt_handler20
.global interrupt_handler21
.global interrupt_handler27

.section .text 


// void interrupt_handler00();
interrupt_handler00:
	push %es 
	push %ds 

	call interrupt_handler_DE
	
	pop %ds 
	pop %es
	iret

// void interrupt_handler01();
interrupt_handler01:
	push %es 
	push %ds 

	call interrupt_handler_DB
	
	pop %ds 
	pop %es
	iret

// void interrupt_handler06();
interrupt_handler06:
	push %es 
	push %ds 

	call interrupt_handler_UD
	
	pop %ds 
	pop %es
	add $4, %esp
	iret

// void interrupt_handler08();
interrupt_handler08:
	push %es 
	push %ds 

	call interrupt_handler_DF
	
	pop %ds 
	pop %es
	add $4, %esp
	iret

// void interrupt_handler0d();
interrupt_handler0d:
	push %es 
	push %ds 

	call interrupt_handler_PF
	
	pop %ds 
	pop %es
	add $4, %esp
	iret

// void interrupt_handler20();
interrupt_handler20:
	push %es 
	push %ds 

	call interrupt_handler_timer
	
	pop %ds 
	pop %es
	iret

// void interrupt_handler21();
interrupt_handler21:
	push %es 
	push %ds 

	call interrupt_handler_keyboard

	pop %ds 
	pop %es 
	iret

// void interrupt_handler27();
interrupt_handler27:
	push %es 
	push %ds 

	call interrupt_handler_irq7
	
	pop %ds 
	pop %es
	iret


;--------------------------------------------------------------------
;	file : libMEM.asm
;
;
;	This file contains subruotines that obtain memory structrues 
;	through calling BIOS interrupts.
;
;	auther: HuangWF
;
;
;-----------------------------------------------------------------

MEMORY_STRUCT_BUFFER_SIZE 	equ 20

;--------------------------------------------------------------------
;	function : getMemoryInfo
;		read sectors from floppy disk.
;	parameters : 
;		di 		: the address of saving memory info
;		
;	return :
;		ax 		: 0 if successful and the number of memory structure
;				  	
;-------------------------------------------------------------------

getMemoryInfo:
	push bp 
	mov bp, sp 
	sub sp, 2

	mov ebx, 0
	mov ax, 0 
	mov es, ax
	mov word [bp-2], 0 

.getMemoryInfoLoop:
	mov eax, 0xe820
	mov ecx, MEMORY_STRUCT_BUFFER_SIZE 
	mov edx, 0x534d4150
	int 15h
	jc .getMemoryInfoFail

	add di, MEMORY_STRUCT_BUFFER_SIZE 
	inc dword [bp-2]

	cmp ebx, 0 
	jne .getMemoryInfoLoop
	jmp .getMemoryInfoSuccessfull

.getMemoryInfoFail:
	mov ax, 0
	jmp .end

.getMemoryInfoSuccessfull:
	mov ax, [bp-2]

.end:
	leave
	ret 

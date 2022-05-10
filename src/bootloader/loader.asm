;--------------------------------------------------------------------
;	file : loader.asm
;
;
;	This file is the booter file which loads necessary file loader.s 
;	into memory and formats the disk in form of FAT12. It will jumps 
;	to loader and continues to execute.
;
;	auther: HuangWF
;
;
;-------------------------------------------------------------------


org	10000h
	jmp start

KERNEL_BASE 					equ 0x00
KERNEL_OFFSET 					equ 0x100000
KERNEL_BASE_TMP					equ 0x00 
KERNEL_OFFSET_TMP				equ 0x7e00

MEMORY_STRUCT_BUFFER_ADDRESS	equ 0x7e00


[SECTION .s16]
[BITS 16]
%include "lib16.asm"
%include "libVBE.asm"
%include "libMEM.asm"

start:
	mov	ax,	cs
	mov	ds,	ax
	mov	es,	ax
	mov	ax,	0x00
	mov	ss,	ax
	mov	sp,	0x7c00

; start loader
	mov byte [ROW], 2 
	mov bp, LOADER_START_MESSAGE
	mov cx, LOADER_START_MESSAGE_LENGTH
	call printMessage

; open address A20 and entry big real mode
	push ax 
	in al, 92h 
	or al, 00000010b
	out 92h, al 
	pop ax 
	cli 
	lgdt [GDT_PTR]
	mov eax, cr0
	or eax, 1 
	mov cr0, eax 
	mov ax, SELECTOR_DATA32
	mov fs, ax 
	mov eax, cr0 
	and al, 11111110b
	mov cr0, eax 
	sti

; search kernel.bin
	mov ax, KERNEL_FILE_NAME
	call searchFile
	cmp ax, 0
	jnz loadingFile
	jmp $

; load loader.bin
loadingFile: 
	push bx
	mov bx, KERNEL_BASE_TMP
	mov es, bx
	mov bx, KERNEL_OFFSET_TMP
loadingFileLoop:
	mov cl, 1
	call readSectors
	
movingKernel:
	push cx 
	push eax
	push fs 
	push edi
	push ds  
	push esi 

	mov cx, 200h 
	mov ax, KERNEL_BASE
	mov fs, ax 
	mov edi, dword [kernelFileOffsetCount]

	mov ax, KERNEL_BASE_TMP
	mov ds, ax 
	mov esi, KERNEL_OFFSET_TMP
movingKernelLoop:
	mov al, byte [ds:esi]
	mov byte [fs:edi], al 
	inc esi 
	inc edi 
	loop movingKernelLoop

	mov eax, 0x1000
	mov ds, eax 
	mov dword [kernelFileOffsetCount], edi

	pop esi 
	pop ds 
	pop edi
	pop fs 
	pop eax 
	pop cx

readNextsector:
	pop ax;
	call getFatEntry
	cmp ax, 0fffh 
	jz fileLoaded
	push ax 
	mov dx, ROOT_DIRECTORY_SECTORS
	add ax, dx 
	add ax, FILE_SECTOR_OFFSET
	jmp loadingFileLoop

fileLoaded:
	mov bp, LOADER_FINISH_MESSAGE 
	mov cx, LOADER_FINISH_MESSAGE_LENGTH
	call printMessage	

; vbe

; get memory info
	mov di, MEMORY_STRUCT_BUFFER_ADDRESS
	call getMemoryInfo
	cmp ax, 0
	jne getMemoryInfoSuccessful
	jmp $

getMemoryInfoSuccessful:
	mov bp, GET_MEMORY_INFO_SUCCESSFUL
	mov cx, GET_MEMORY_INFO_SUCCESSFUL_LENGTH
	call printMessage

; clear screen
	call clearScreen

; jump to protected mode
	cli
	lgdt [GDT_PTR]
	mov eax, cr0
	and eax, 0x7fffffff		; disable paging
	or eax, 1           	; switch to protected mode
	mov cr0, eax 
	jmp dword SELECTOR_CODE32:protectedMode 

[SECTION .s32]
[BITS 32]
protectedMode:
	mov ax, 0x10 
	mov ds, ax 
	mov es, ax 
	mov fs, ax 
	mov ss, ax 
	mov esp, 7e00h

; jmp to kernel
	jmp SELECTOR_CODE32:KERNEL_OFFSET

[SECTION gdt]
GDT_START:			dd 0, 0 
DESCRIPTOR_CODE32:	dd 0x0000FFFF, 0x00CF9A00
DESCRIPTOR_DATA32:	dd 0x0000FFFF, 0x00CF9200

GDT_LENGTH 			equ $ - GDT_START
GDT_PTR: 			dw GDT_LENGTH - 1
					dd GDT_START
SELECTOR_CODE32 	equ DESCRIPTOR_CODE32 - GDT_START
SELECTOR_DATA32		equ DESCRIPTOR_DATA32 - GDT_START

; variables
kernelFileOffsetCount 				dd KERNEL_OFFSET

;message labels
KERNEL_FILE_NAME					db "KERNEL  BIN"
KERNEL_FILE_NAME_LENGTH 			equ $ - KERNEL_FILE_NAME
LOADER_START_MESSAGE:				db "loader start"
LOADER_START_MESSAGE_LENGTH 		equ $ - LOADER_START_MESSAGE
LOADER_FINISH_MESSAGE 				db "loader finish"
LOADER_FINISH_MESSAGE_LENGTH 		equ $ - LOADER_FINISH_MESSAGE
GET_MEMORY_INFO_SUCCESSFUL			db "get memory info successful"
GET_MEMORY_INFO_SUCCESSFUL_LENGTH 	equ $ - GET_MEMORY_INFO_SUCCESSFUL



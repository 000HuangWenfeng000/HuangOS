;--------------------------------------------------------------------
;	file : boot.asm
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
	
	org 0x7c00

; labels
STACK_BASE 				equ 0x7c00
LOADER_BASE 			equ 0x1000
LOADER_OFFSET 			equ 0

	jmp start
	%include "lib16.asm"

start:		
;set segment registers
	;xor ax, ax
	;mov cs, ax
	mov ax, cs 
	mov ds, ax 
	mov es, ax 
	mov ss, ax 
	mov sp, STACK_BASE

initializeScreen:
; clear screen	
	mov ax, 0600h
	mov bx, 0700h
	mov cx, 0
	mov dx, 0184fh
	int 10h

; set focus
	mov ax, 0200h
	mov bx, 0000h
	mov dx, 0000h
	int 10h

; print message
	mov cx, BOOT_START_MESSAGE_LENGTH
	mov bp, BOOT_START_MESSAGE
	call printMessage

; search loader.bin
	mov ax, LOADER_FILE_NAME
	call searchFile
	cmp ax, 0
	jnz loadingFile
	jmp $

; load loader.bin
loadingFile: 
	push bx
	mov bx, LOADER_BASE
	mov es, bx
	mov bx, LOADER_OFFSET
loadingFileLoop:
	mov cl, 1
	call readSectors
	pop ax
	call getFatEntry
	cmp ax, 0fffh 
	jz fileLoaded
	push ax 
	mov dx, ROOT_DIRECTORY_SECTORS
	add ax, dx 
	add ax, FILE_SECTOR_OFFSET
	add bx, [BPB_BytesPerSec]
	jmp loadingFileLoop

fileLoaded:
; print message
	mov cx, BOOT_FINISH_MESSAGE_LENGTH
	mov bp, BOOT_FINISH_MESSAGE
	call printMessage

; jmp to loader
	jmp LOADER_BASE:LOADER_OFFSET

; message labels
BOOT_START_MESSAGE: 		db "Boot"
BOOT_START_MESSAGE_LENGTH 	equ $ - BOOT_START_MESSAGE
BOOT_FINISH_MESSAGE: 		db "Done"
BOOT_FINISH_MESSAGE_LENGTH 	equ $ - BOOT_FINISH_MESSAGE
LOADER_FILE_NAME 			db "LOADER  BIN"
LOADER_FILE_NAME_LENGTH 	equ $ - LOADER_FILE_NAME

	times 510 - ($ - $$) db 0
	dw 0xaa55
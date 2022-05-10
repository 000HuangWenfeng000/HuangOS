;--------------------------------------------------------------------
;	file : lib16.asm
;
;
;	This file is used by both boot and loader. It mainly consists of 
;	subroutines related to FAT12 file system.
;
;	auther: HuangWF
;
;
;-------------------------------------------------------------------
%include "fat12.inc"

ROW 							db 0
FILE_NOT_FOUND_MESSAGE 			db "E"
FILE_NOT_FOUND_MESSAGE_LENGTH 	equ $ - FILE_NOT_FOUND_MESSAGE
FAT_ENTRIES_PER_SECTOR 			equ 16
TMP_ADDRESS_OF_SECTORS			equ 0x8000


;--------------------------------------------------------------------
;	function : printMessage
;		read sectors from floppy disk.
;	parameters : 
;		cx 		: message address
;		bx 		: message length
;	return :
;
;-------------------------------------------------------------------
printMessage:
	push ax 
	push bx 
	push cx 
	push dx 
	push bp 

	mov ax, ds 
	mov es, ax
	mov ax, 1301h
	mov bx, 000fh
	mov dh, [ROW]
	inc byte [ROW]
	mov dl, 00h
	int 10h
	
	pop bp 
	pop dx 
	pop cx 
	pop bx 
	pop ax
	ret 

;--------------------------------------------------------------------
;	function : readSectors
;		read sectors from floppy disk.
;	parameters : 
;		ax 		: sector from which is read
;		cl 		: the numbers to be read
;		es:bx 	: target address
;	return :	
;
;-------------------------------------------------------------------
readSectors:
	push bp 
	mov bp, sp 
	sub sp, 2
	push dx
	push bx  

	mov byte [bp-2], cl
	mov bl, [BPB_SecPerTrk]
	div bl 
	inc ah 
	mov cl, ah 
	mov dh, al 
	shr al, 1
	mov ch, al 
	and dh, 1 
	mov dl, [BS_DrvNum]
.reading:
	pop bx
	mov ah, 2 
	mov al, byte [bp-2]
	int 0x13
	jc .reading
	
	pop dx 
	mov sp, bp
	pop bp
	ret

;--------------------------------------------------------------------
;	function : getFatEntry
;		read sectors from floppy disk.
;	parameters : 
;		ax 		: fat entry number
;	return :
;		ax 		: next fat entry number
;
;-------------------------------------------------------------------
getFatEntry:
	push bp
	mov bp, sp
	sub sp, 2 
	push bx 
	push es

	push ax 
	mov ax, 0 
	mov es, ax 
	pop ax 
	mov byte [bp-2], 0 
	mov bx, 3 
	mul bx 
	mov bx, 2 
	div bx 
	cmp dx, 0 
	jz .even
	mov byte [bp-2], 1
.even:
	xor dx, dx 
	mov bx, [BPB_BytesPerSec]
	div bx
	push dx   
	mov bx, TMP_ADDRESS_OF_SECTORS 
	add ax, FAT1_SECTOR_OFFSET 
	mov cl, 2
	call readSectors
	pop dx
	add bx, dx
	mov ax, [es:bx]
	mov bl, byte [bp-2]
	cmp bl, 1 
	jnz .finish 
	shr ax, 4
.finish:
	and ax, 0fffh 

	pop es 
	pop bx 
	add sp, 2
	pop bp  
	ret

;--------------------------------------------------------------------
;	function : searchFile
;		read sectors from floppy disk.
;	parameters : 
;		ax 	: address of filename
;	return :
;		ax : 0 fail, not 0 successfully find fisrt fat entry
;		bx : 
;
;-------------------------------------------------------------------
searchFile:
	push bp 
	mov bp, sp 
	sub sp, 6
	push cx 
	push dx 
	push si 
	push di 
	
	mov word [bp-2], ROOT_DIRECTORY_SECTORS 
	mov word [bp-4], ROOT_DIRECTORY_SECTOR_OFFSET
	mov [bp-6], ax 			; filename
 
.searchBegin:
	mov  ax, [bp-2]
	cmp ax, 0
	jz .fileNotFound
	dec word [bp-2]
	mov ax, 0 
	mov es, ax 
	mov bx, TMP_ADDRESS_OF_SECTORS
	mov ax, [bp-4]
	mov cl, 1
	call readSectors
	mov si, [bp-6]
	mov di, TMP_ADDRESS_OF_SECTORS
	mov dx, FAT_ENTRIES_PER_SECTOR

.searchFilename:
	cmp dx, 0 
	jz .nextSector
	dec dx 
	mov cx, 11 

.compareFilename:
	cmp cx, 0
	jz .fileFound
	dec cx 
	mov al, [si] 
	cmp al, byte [es:di]
	jz .nextChar
	jmp .nextFile

.nextChar:
	inc si
	inc di 
	jmp .compareFilename

.nextFile:
	and di, 0ffe0h 
	add di, 20h 
	mov si, [bp-6]
	jmp .searchFilename

.nextSector:
	add word [bp-4], 1
	jmp .searchBegin

.fileNotFound:
	push bp 
	mov cx, FILE_NOT_FOUND_MESSAGE_LENGTH
	mov bp, FILE_NOT_FOUND_MESSAGE
	call printMessage 
	pop bp
	mov ax, 0
	mov bx, 0
	jmp .end

.fileFound:
	mov ax, ROOT_DIRECTORY_SECTORS
	and di, 0ffe0h 
	add di, 01ah 
	mov cx, word [es:di] 
	mov bx, cx
	add cx, ax 
	add cx, FILE_SECTOR_OFFSET 
	mov ax, cx
	 
.end:
	pop di 
	pop si 
	pop dx 
	pop cx 
	mov sp, bp 
	pop bp 
	ret

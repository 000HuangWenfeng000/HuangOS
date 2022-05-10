;--------------------------------------------------------------------
;	file : libVBE.asm
;
;
;	VBE
;
;	auther: HuangWF
;
;
;-----------------------------------------------------------------

;--------------------------------------------------------------------
;	function : 
;		clear screen and set coursor
;	parameters : 
;	return :
;		
;
;-------------------------------------------------------------------
clearScreen:
	push ax
	push bx
	push cx 
	push dx

; clear screen
	mov ax, 0600h
	mov bx, 0700h
	mov cx, 0
	mov dx, 0184fh
	int 10h

; set focus
	mov ax, 0200h
	mov bx, 0000h
	mov dx, 0ffffh
	int 10h

	pop dx 
	pop cx 
	pop bx 
	pop ax
	ret 

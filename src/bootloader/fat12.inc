;--------------------------------------------------------------------
;	file : fat12.inc
;
;
;	fat12 file system and related variables.
;
;	auther: HuangWF
;
;
;--------------------------------------------------------------------
	

ROOT_DIRECTORY_SECTORS			equ 14
ROOT_DIRECTORY_SECTOR_OFFSET 	equ 19
FAT1_SECTOR_OFFSET				equ 1
FILE_SECTOR_OFFSET				equ 17

	BS_OEMNAME			db "HuangOS "
	BPB_BytesPerSec		dw 512
	BPB_SecPerClus		db 1
	BPB_RsvdSecCnt		dw 1
	BPB_NumFATs			db 2
	BPB_RootEntCnt		dw 224
	BPB_TotSec16		dw 2880
	BPB_Media			db 0xf0
	BPB_FATsz16			dw 9
	BPB_SecPerTrk		dw 18
	BPB_NumHeads 		dw 2
	BPB_HiddSec			dd 0
	BPB_TotSec32		dd 0
	BS_DrvNum 			db 0
	BS_Reserved1		db 0 
	BS_BootSig 			db 29
	BS_VolID			db 0
	BS_VolLab			db "boot loader"
	BS_FileSysType		db "FAT12   "

; os boot asm
; TAB=4

BOTPAK	EQU		0x00280000		; bootpack의 로드 장소
DSKCAC	EQU		0x00100000		; 디스크 캐쉬 프로그램의 장소
DSKCAC0	EQU		0x00008000		; 디스크 캐쉬 프로그램의 장소(리얼모드)

; BOOT INFO binfo
; 保存在 0x0ff0 附近，这里的内存未被使用

CYLS  EQU 0x0ff0               ; 启动区
LEDS  EQU 0x0ff1               ;
VMODE EQU 0x0ff2               ; 颜色模式 位数
SCRNX EQU 0x0ff4               ; screen x 分辨率 X 值
SCRNY EQU 0x0ff6               ; screen y 分辨率 Y 值
VRAM  EQU 0x0ff8               ; 图像缓冲区起始地址

[bits 16]

; 程序被装载的位置

ORG 0xc200

;[org 0x8200]

; 

		MOV		AL, 0x13	; VGA 显卡 320 x 200 8 位彩色
		MOV		AH, 0x00
		INT		0x10
		MOV		BYTE [VMODE], 8	; 画面模式
		MOV		WORD [SCRNX], 320
		MOV		WORD [SCRNY], 200
		MOV		DWORD [VRAM], 0x000a0000

; 用 BIOS 取得各种 LED 指示灯状态

		MOV		AH, 0x02
		INT		0x16 		; keyboard BIOS
		MOV		[LEDS], AL

fin:
	  HLT
		JMP fin

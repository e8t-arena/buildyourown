; os.nas

; BOOT INFO
; 保存在 0x0ff0 附近，这里的内存未被使用
CYLS  EQU 0x0ff0               ; 启动区
LEDS  EQU 0x0ff1               ; 
VMODE EQU 0x0ff2               ; 颜色模式 位数
SCRNX EQU 0x0ff4               ; screen x 分辨率 X 值
SCRNY EQU 0x0ff6               ; screen y 分辨率 Y 值
VRAM  EQU 0x0ff8               ; 图像缓冲区起始地址

  ; 0x8000+0x4200
  ORG 0xc200

  MOV AL, 0x13                 ; 设置 VGA 显卡 320x200 8位彩色
  MOV AH, 0x00 
  INT 0x10

  MOV BYTE [VMODE], 8          ; 记录画面模式
  MOV WORD [SCRNX], 320        ;
  MOV WORD [SCRNY], 200
  ; 0x000a0000 是 BIOS 函数中设定的，8 位彩色模式下
  ; VRAM 0xa0000 - 0xafffff 64k 大小
  MOV DWORD [VRAM], 0x000a0000

  ; 获得键盘 LED 指示灯状态
  MOV AH, 0x02 
  INT 0x16
  MOV [LEDS], AL

fin:
  HLT
  JMP fin

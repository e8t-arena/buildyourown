CYLS EQU 10
ORG 0x7c00
JMP entry
DB 0x90
DB "TINYOS  "
DW 512
DB 1
DW 1              ; FAT 起始位置
DB 2 
DW 224
DW 2880
DB 0xf0
DW 9
DW 18
DW 2
DD 0 
DD 2880
DB 0,0,0x29
DD 0xffffffff
DB "TINYOS     "
DB "FAT12   "
RESB 18           ; 空出 18 bytes

; Body

entry:
  MOV AX, 0
  MOV SS, AX 
  MOV SP, 0x7c00
  MOV DS, AX 

  ; 加载下一个 512 字节内容
  MOV AX, 0x0820
  MOV ES, AX
  MOV CH, 0       ; 柱面 0
  MOV DH, 0       ; 磁头 0
  MOV CL, 2       ; 扇区 2

readloop:
  MOV SI, 0       ; 记录尝试读取扇区失败次数

retry:
  MOV AH, 0x02    ; AH = 0x02 读取扇区
  MOV AL, 1       ; 读取一个扇区
  MOV BX, 0 
  MOV DL, 0x00    ; A 驱动器

  INT 0x13
  JNC next        ; 读取正常，进入 fin / next

  ADD SI, 1       ; 读取出错，SI + 1
  CMP SI, 5       ; 设置最多尝试次数: 5
  JAE error       ; SI >=5 进入 error

  MOV AH, 0x00    ; SI <5 继续尝试读取，重置读取配置
  MOV DL, 0x00
  INT 0x13
  JMP retry

; 逐个扇区读取，将磁盘加载到内存 0x8200 - 0xa3ff
; 扇区读到 C0-H1-S18
; 下一个将是 C1-H0-S1
next:
  MOV AX, ES      ; 内存地址后移 0x200
  ADD AX, 0x0020  
  MOV ES, AX      ; 利用 AX 完成后移
  ADD CL, 1       ; 扇区号 + 1
  CMP CL, 18      ; 读取到 18 停止
  JBE readloop    ; <= 18 跳转到 readloop
  
  MOV CL, 1
  ADD DH, 1
  CMP DH, 2
  JB  readloop    ; DH < 2 继续读取下一个磁头

  MOV DH, 0       ; DH = 2 继续读取下一个柱面
  ADD CH, 1
  CMP CH, CYLS 
  JB  readloop

  MOV [0x0ff0], CH  ; CH 值等于 CYLS
  JMP 0xc200        ; 跳转到 os.nas 加载位置

error:
  MOV SI, errormsg 

loop:
  MOV AL, [SI]
  ADD SI, 1
  CMP AL, 0 
  JE  fin
  MOV AH, 0x0e    ; 显示文字
  MOV BX, 15      ; 设置字符颜色
  INT 0x10        ; 显示 (调用显卡)
  JMP loop 

fin:
  HLT
  JMP fin 

errormsg:
  DB 0x0a, 0x0a   ; 两次换行
  DB "LOAD ERROR"
  DB 0x0a 
  DB 0

; 用于识别为操作系统
; RESB 0x1fe-$
; times 510 - ($-$$) db 0 ; 一种方法
RESB 0x1fe-($-$$)
DB 0x55, 0xaa

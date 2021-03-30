; 描述 标准FAT12 格式

ORG 0x7c00        ; 装载程序的内存位置

JMP entry         ; 跳转到下边 entry 标签位置

DB 0x90
DB "TINYOS  "     ; 启动区名字 8 byte 内任意 **不足 8 需要补全
DW 512            ; 单个扇区 sector 大小 必须是 512 bytes
DB 1              ; 簇 cluster 大小 必须为 1 个扇区
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

  MOV SI, 0       ; 记录尝试读取扇区失败次数

retry:
  MOV AH, 0x02    ; AH = 0x02 读取扇区
  MOV AL, 1       ; 读取一个扇区
  MOV BX, 0 
  MOV DL, 0x00    ; A 驱动器

  INT 0x13
  JNC fin     ; 读取正常，进入 fin 

  ADD SI, 1       ; 读取出错，SI + 1
  CMP SI, 5       ; 设置最多尝试次数: 5
  JAE error       ; SI >=5 进入 error

  MOV AH, 0x00    ; SI <5 继续尝试读取，重置读取配置
  MOV DL, 0x00
  INT 0x13
  JMP retry

loop:
  MOV AL, [SI]
  ADD SI, 1
  CMP AL, 0 
  JE  fin
  MOV AH, 0x0e    ; 显示文字
  MOV BX, 15      ; 设置字符颜色
  INT 0x10        ; 显示 (调用显卡)
  JMP loop 

error:
  MOV SI, errormsg 

fin:
  HLT
  JMP fin 

errormsg:
  DB 0x0a, 0x0a   ; 两次换行
  DB "LOAD ERROR"
  DB 0x0a 
  DB 0

display:
  MOV SI, msg 
  JMP loop

msg:
  DB 0x0a, 0x0a   ; 两次换行
  DB "LOAD floppy"
  DB 0x0a 
  DB 0

; RESB 0x1fe-$
; 用于识别为操作系统
RESB 0x1fe-($-$$)
DB 0x55, 0xaa

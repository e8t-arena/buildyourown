; 描述 标准FAT12 格式

ORG 0x7c00 ; 装载程序的内存位置

JMP entry ; 跳转到下边 entry 标签位置

; DB 0xeb, 0x4e, 0x90
DB 0x90
; 启动区名字 8 byte 内任意 **不足 8 需要补全
; 8 bytes = 8 characters (in ASCII Code)
; In general, 1 byte = 1 ASCII character. 2 bytes = 1 UTF-16 character.
DB "TINYOS  "
; 单个扇区 sector 大小 必须是 512 bytes
DW 512
; 簇 cluster 大小 必须为 1 个扇区
DB 1
; FAT 起始位置
DW 1
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
; 空出 18 bytes
RESB 18

; Body

entry:
  MOV AX, 0
  MOV SS, AX 
  MOV SP, 0x7c00
  MOV DS, AX 

  ; 加载下一个 512 字节内容
  MOV AX, 0x0820
  MOV ES, AX
  MOV CH, 0     ; 柱面 0
  MOV DH, 0     ; 磁头 0
  MOV CL, 2     ; 扇区 2

  MOV AH, 0x02  ; AH = 0x02 读取扇区
  MOV AL, 1     ; 读取一个扇区
  MOV BX, 0 
  MOV DL, 0x00  ; A 驱动器

  INT 0x13
  JC  error

loop:
  MOV AL, [SI]
  ADD SI, 1
  CMP AL, 0 
  JE  fin
  MOV AH, 0x0e  ; 显示文字
  MOV BX, 15    ; 设置字符颜色
  INT 0x10      ; 显示 (调用显卡)
  JMP loop 

error
  MOV SI, msg 

fin:
  HLT
  JMP fin 

msg:
  DB 0x0a, 0x0a  ; 两次换行
  DB "LOAD ERROR"
  DB 0x0a 
  DB 0

; RESB 0x1fe-$
; 用于识别为操作系统
RESB 0x1fe-($-$$)
DB 0x55, 0xaa

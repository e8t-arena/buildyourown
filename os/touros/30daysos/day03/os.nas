; haribote.nas

  ; 0x8000+0x4200
  ORG 0xc200

  MOV AL, 0x13                 ; 设置 VGA 显卡 320x200 8位彩色
  MOV AH, 0x00 
  INT 0x10

fin:
  HLT
  JMP fin

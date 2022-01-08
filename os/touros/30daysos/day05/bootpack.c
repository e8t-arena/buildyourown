/*
 * bootpack.c
 * Copyright (C) 2021 Peter Lau <superpeterlau@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
/* #include "bootpack.h" */

#define COL8_000000 0
#define BLACK 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define PURPLE 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

#define UI_SIZE 5

typedef struct {
  char cyls, leds, vmode, reserve;
  short scrnx, scrny;
  unsigned char *vram;
} BootInfo;

// 引入汇编函数

void io_hlt(void);  // 函数声明
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

// 调色板相关函数声明

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);

void write_mem8(int addr, int data);

void draw_bars(unsigned char *vram);

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void draw_more(unsigned char *vram, int xsize, int (*ui)[UI_SIZE], int ui_size);

void init_screen(unsigned char *vram, int xsize, int ysize);

void init_mouse_cursor8(char *mouse, char bc);

void putfont8(unsigned char *vram, int xsize, int x, int y, char color, unsigned char *font);

void putfonts8_asc(unsigned char *vram, int xsize, int x, int y, char color, char *s);

void putfonts8_asc_binfo(BootInfo *binfo, int x, int y, char c, char *s);

void putblock8_8(BootInfo *binfo, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize);

void OSMain(void)
{
  unsigned char *vram;
  // int xsize, ysize;
  // struct BootInfo *binfo;
  BootInfo *binfo;

  // 设定调色板
  init_palette();

  // 需要初始化指针变量
  // 即 binfo = 0xfff0; // asmhead.nas 中 CYLS 启动区的位置
  // binfo = (struct BootInfo *) 0x0ff0;
  binfo = (BootInfo *) 0x0ff0;

  // 不能省略为 *binfo.scrnx 会被解释为 *(binfo.scrnx)
  // xsize = (*binfo).scrnx;
  // ysize = (*binfo).scrny;
  vram = (*binfo).vram;

  // init_screen(vram, xsize, ysize);

  init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

  // display font A
  static unsigned char font_A[16] = {
    0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24,
    0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x00, 0x00
  };
  putfont8(vram, binfo->scrnx, 0, 0, BLACK, font_A);

  // char data[] = "Hello TinyOS";
  putfonts8_asc(binfo->vram, binfo->scrnx, 8,  8, BLACK, "Hello TinyOS");
  putfonts8_asc(binfo->vram, binfo->scrnx, 0, 24, PURPLE, "Convert String to ASCII Value in Python");

  // char *s = NULL;
  char s[40];
  sprintf(s, "scrnx = %d", binfo->scrnx);
  putfonts8_asc_binfo(binfo, 0, 32, PURPLE, s);

  // boxfill8(vram, 320, COL8_FF0000, 20, 20, 120, 120);
  // boxfill8(vram, 320, COL8_00FF00, 70, 50, 170, 150);
  // boxfill8(vram, 320, COL8_0000FF, 120, 80, 220, 180);

  char mcursor[256];
  int mx, my;
  mx = (binfo->scrnx - 16)/2;
  my = (binfo->scrny - 28 - 16)/2;
  init_mouse_cursor8(mcursor, COL8_008484);
  putblock8_8(binfo, 16, 16, mx, my, mcursor, 16);

  for (;;) {
    io_hlt();
  }
/* fin: */
/*   goto fin; */
}

void draw_bars(unsigned char *p) {
  int i; 

  // mov byte [i], 15
  // 15 是全部颜色的第十五种颜色，即纯白色
  
  /*
  for (i = 0xa0000; i <= 0xaffff; i++) {
    // p 未赋值为 0xa0000 时需要从 i = 0xa0000 开始
    write_mem8(i, i & 0x0f);
    // 以下等价于 write_mem8
    // p = i; // 未指明类型 warning
    p = (char *) i; 
    *p = i & 0x0f; 
  }
  */
  
  for (i = 0; i <= 0xffff; i++) {
    // 等价的两种写法
    // *(p + i) = i & 0x0f;
    p[i] = i & 0x0f;
  }
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1) {
  int x, y;
  for (y=y0; y<=y1; y++) {
    for (x=x0; x<=x1; x++) {
      vram[x + y * xsize] = c;
    }
  }
}

void draw_more(unsigned char *vram, int xsize, int (*ui)[UI_SIZE], int ui_size) {
  // int row;
  // row = (int) (sizeof(*ui)/sizeof((*ui)[0]));
  // printf("length of ui: %d\n", row);

  for(int i = 0; i < ui_size; i++)
      // boxfill8(vram, xsize, (*ui)[i][0], 0, 0, xsize-1,ysize-29)
      // boxfill8(vram, xsize, (*ui)[i][0], (*ui)[i][1], (*ui)[i][2], (*ui)[i][3], (*ui)[i][4]);
      boxfill8(vram, xsize, ui[i][0], ui[i][1], ui[i][2], ui[i][3], ui[i][4]);
}

void init_screen(unsigned char *vram, int xsize, int ysize) {
  int ui[][5] = {
    {COL8_008484,        0,        0, xsize-1, ysize-29},
    {COL8_C6C6C6,        0, ysize-28, xsize-1, ysize-28},
    {COL8_FFFFFF,        0, ysize-27, xsize-1, ysize-27},
    {COL8_C6C6C6,        0, ysize-26, xsize-1, ysize-1},
    {COL8_FFFFFF,        3, ysize-24,      59, ysize-24},
    // {COL8_000000,        2, ysize-24,       2, ysize-4},
    {COL8_FFFFFF,        2, ysize-24,       2, ysize-4},
    {COL8_848484,        3, ysize-4,       59, ysize-4},
    {COL8_848484,       59, ysize-23,      59, ysize-5},
    {COL8_000000,        2, ysize-3,       59, ysize-3},
    {COL8_000000,       60, ysize-24,      60, ysize-3},
    {COL8_848484, xsize-47, ysize-24, xsize-4, ysize-24},
    {COL8_848484, xsize-47, ysize-23, xsize-47,ysize-4},
    {COL8_FFFFFF, xsize-47, ysize-3,  xsize-4, ysize-3},
    {COL8_FFFFFF, xsize-3,  ysize-24, xsize-3, ysize-3}
  };

  int ui_size;
  ui_size = (int) (sizeof(ui) / sizeof(ui[0]));
  draw_more(vram, xsize, ui, ui_size);
}

void init_palette(void) {
  // static 相当于 DB 只用于数据
  static unsigned char rgb_table[16 * 3] = {
    0x00, 0x00, 0x00, //  0:黑
    0xff, 0x00, 0x00, //  1:亮红
    0x00, 0xff, 0x00, //  2:亮绿
    0xff, 0xff, 0x00, //  3:亮黄
    0x00, 0x00, 0xff, //  4:亮蓝
    0xff, 0x00, 0xff, //  5:亮紫
    0x00, 0xff, 0xff, //  6:浅蓝色
    0xff, 0xff, 0xff, //  7:白色
    0xc6, 0xc6, 0xc6, //  8:亮灰
    0x84, 0x00, 0x00, //  9:暗红
    0x00, 0x84, 0x00, // 10:暗绿
    0x84, 0x84, 0x00, // 11:暗黄
    0x00, 0x00, 0x84, // 12:暗青
    0x84, 0x00, 0x84, // 13:暗紫
    0x00, 0x84, 0x84, // 14:浅暗蓝
    0x84, 0x84, 0x84, // 15:暗灰
  };
  set_palette(0, 15, rgb_table);
  return;
}

void set_palette(int start, int end, unsigned char *rgb) {
  int i, eflags;
  eflags = io_load_eflags(); // 记录中断标识值
  io_cli();                  // 设置中断标识为 0 禁止中断
  io_out8(0x03c8, start);
  for (i = start; i<= end;i++) {
    io_out8(0x03c9, rgb[0] / 4);
    io_out8(0x03c9, rgb[1] / 4);
    io_out8(0x03c9, rgb[2] / 4);
    rgb += 3;
  }
  io_store_eflags(eflags);   // 恢复中断值
  return;
}

// int x, int y 位置坐标
// char c 是 color

void putfont8(unsigned char *vram, int xsize, int x, int y, char c, unsigned char *font) {
  int i;
  unsigned char *p, data;
  // 逐行显示
  for (i = 0; i < 16; i++) {
    data = font[i];

    // 0x80 : 0b 1000 0000
    // if( (d & 0x80) != 0 ) { vram[(y+i) * xsize + x + 0] = c; }

    p = vram + (y + i) * xsize + x;
    if( (data & 0x80) != 0 ) { p[0] = c;}
    if( (data & 0x40) != 0 ) { p[1] = c;}
    if( (data & 0x20) != 0 ) { p[2] = c;}
    if( (data & 0x10) != 0 ) { p[3] = c;}
    if( (data & 0x08) != 0 ) { p[4] = c;}
    if( (data & 0x04) != 0 ) { p[5] = c;}
    if( (data & 0x02) != 0 ) { p[6] = c;}
    if( (data & 0x01) != 0 ) { p[7] = c;}
  }
}

// unsigned char *s : 表示 text 字符串

void putfonts8_asc(unsigned char *vram, int xsize, int x, int y, char c, char *s) {
  unsigned char *data;
  data = (unsigned char *)s;
  extern unsigned char hankaku[4096];
  // 指针增加
  // 字符串末尾是 0x00
  for (; *data != 0x00; data++) {
    putfont8(vram, xsize, x, y, c, hankaku + *data * 16);
    // 横向排列
    x += 8;
  }
  return;
}

void putfonts8_asc_binfo(BootInfo *binfo, int x, int y, char c, char *s) {
  putfonts8_asc(binfo->vram, binfo->scrnx, x, y, c, s);
}

void init_mouse_cursor8(char *mouse, char bc) {
  // 16 x 16 鼠标
  static char cursor[16][16] = {
    "**************..",
    "*OOOOOOOOOOO*...",
    "*OOOOOOOOOO*....",
    "*OOOOOOOOO*.....",
    "*OOOOOOOO*......",
    "*OOOOOOO*.......",
    "*OOOOOOO*.......",
    "*OOOOOOOO*......",
    "*OOOO**OOO*.....",
    "*OOO*..*OOO*....",
    "*OO*....*OOO*...",
    "*O*......*OOO*..",
    "**........*OOO*.",
    "*..........*OOO*",
    "............*OO*",
    ".............***",
  };
  int x, y;
  for(y = 0; y < 16; y++) {
    for(x = 0; x < 16; x++) {
      if (cursor[y][x] == '*') {
        mouse[y * 16 + x] = COL8_000000;
      }
      if (cursor[y][x] == 'O') {
        mouse[y * 16 + x] = COL8_FFFFFF;
      }
      if (cursor[y][x] == '.') {
        mouse[y * 16 + x] = bc;
      }
    }
  }
  return;
}

// buf 图形存放地址
// bxsize 每行像素数
void putblock8_8(BootInfo *binfo, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize) {
  int x, y;
  for(y = 0; y < pysize; y++) {
    for(x = 0; x < pxsize; x++) {
      binfo->vram[(py0 + y) * binfo->scrnx + (px0 + x)] = buf[y * bxsize + x];
    }
  }
  return;
}

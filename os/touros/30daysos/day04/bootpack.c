/*
 * bootpack.c
 * Copyright (C) 2021 Peter Lau <superpeterlau@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

/* #include "bootpack.h" */

// 找不到路径 没有头文件
// #include <stdio.h>

#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
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

void OSMain(void)
{
  // int i; 
  unsigned char *p;
  int xsize=320, ysize=200;

  // 设定调色板
  init_palette();

  p = (unsigned char *) 0xa0000;

  draw_bars(p);

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
  draw_more(p, xsize, ui, ui_size);

  boxfill8(p, 320, COL8_FF0000, 20, 20, 120, 120);
  boxfill8(p, 320, COL8_00FF00, 70, 50, 170, 150);
  boxfill8(p, 320, COL8_0000FF, 120, 80, 220, 180);

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

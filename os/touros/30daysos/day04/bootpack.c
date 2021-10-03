/*
 * bootpack.c
 * Copyright (C) 2021 Peter Lau <superpeterlau@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

/* #include "bootpack.h" */

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

void OSMain(void)
{
  int i; 
  char *p;

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

  p = (char *) 0xa0000;
  
  for (i = 0; i <= 0xffff; i++) {
    // 等价的两种写法
    // *(p + i) = i & 0x0f;
    p[i] = i & 0x0f;
  }

  for (;;) {
    io_hlt();
  }
/* fin: */
/*   goto fin; */
}


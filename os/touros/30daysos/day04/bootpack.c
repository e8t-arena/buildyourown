/*
 * bootpack.c
 * Copyright (C) 2021 Peter Lau <superpeterlau@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

/* #include "bootpack.h" */

void io_hlt(void);  // 函数声明

void write_mem8(int addr, int data);

void HariMain(void)
{
  int i; 

  // mov byte [i], 15
  // 15 是全部颜色的第十五种颜色，即纯白色
  for (i = 0xa0000; i <= 0xaffff; i++) {
    write_mem8(i, i & 0x0f);
  }

  for (;;) {
    io_hlt();
  }
/* fin: */
/*   goto fin; */
}

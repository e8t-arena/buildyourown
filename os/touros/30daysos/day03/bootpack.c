/*
 * bootpack.c
 * Copyright (C) 2021 Peter Lau <superpeterlau@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

/* #include "bootpack.h" */

void io_hlt(void);  // 函数声明

void HariMain(void)
{
  for (;;) {
    io_hlt();
  }
/* fin: */
/*   goto fin; */
}

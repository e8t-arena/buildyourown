/*
 * play.c
 * Copyright (C) 2021 Peter Lau <superpeterlau@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

/* #include "play.h" */

#include <stdio.h>
#include <stdint.h>

void log_bits(int flag) {
  for (int i = 31; i >= 0; i--)
    /* std::cout << ((v >> i) & 1); */
    /* printf("v: %d\n", v>>i&1); */
    printf("%d", flag >> i & 1);
  printf("\n");
}

void bitwise() {
  int flag = 0b00000000000000000000000100000000;
  int flag1 = 0b00000000000000000000000000001000;
  log_bits(flag | flag1);
  log_bits(flag);
  flag &= ~(flag);
  log_bits(flag);
  flag &= ~(flag);
  log_bits(flag);
}

void print_hex() {
  /* int flag = 0b00000000000000000000000000001000; */ 
  /* printf("flag: %b\n", flag); */
  int v = 0x12345678;
  v = 0x1234;
  for (int i = 31; i >= 0; i--)
    /* std::cout << ((v >> i) & 1); */
    /* printf("v: %d\n", v>>i&1); */
    printf("%d", v >> i & 1);
  printf("\n");
  /* int flag = 0x1234; */
  /* printf("flag: %04x\n", flag); */
  /* printf("flag: 0x%02X\n", flag); */
}

void mask(uint32_t value) {
  uint32_t byte1 = (value >> 24);
  uint32_t byte2 = (value >> 16) & 0xff;
  uint32_t byte3 = (value >> 8) & 0xff;
  // 改变顺序需要用不同的 mask
  // uint32_t byte3 = (value & 0xff00) >> 8;
  uint32_t byte4 = value & 0xff;
  printf("%d\n", byte1);
  printf("%d\n", byte2);
  printf("%d\n", byte3);
  printf("%d\n", byte4);
}

int main()
{
  char msg[] = {'c','9','9','\n', '\0'};
  printf("%s", msg);

  bitwise();
  mask(0x0102030405);

  return 0;
}

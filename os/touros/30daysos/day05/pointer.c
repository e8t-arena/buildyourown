#include <stdio.h>

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


void cal_size(int (*ui)[5]);

int main() { int v1;
  char v2[10];

  printf("Address of v1: %x\n", (unsigned int) &v1);
  printf("Address of v2: %x\n", (unsigned int) &v2);

  int v3 = 20;
  int *iv3;
  iv3 = &v3;
  printf("Address of v3: %x\n", (unsigned int) &v3);

  printf("Address stored in iv3: %x\n", (unsigned int) iv3);

  printf("Address stored in iv3 & 0x0f: %x\n", (unsigned int) ((int) iv3 & 0xab));

  printf("Value of *iv3 :%d\n", *iv3);


  int ui[][5] = {
    {1,2,3,4,5},
    {1,2,3,4,5},
    {1,2,3,4,5},
    {1,2,3,4,5},
    {1,2,3,4,5},
  };
  cal_size(ui);

  int xsize=320, ysize=200;
  int u[][5] = {
    {COL8_008484,        0,        0, xsize-1, ysize-29},
    {COL8_C6C6C6,        0, ysize-28, xsize-1, ysize-28},
    {COL8_FFFFFF,        0, ysize-27, xsize-1, ysize-27},
    {COL8_C6C6C6,        0, ysize-26, xsize-1, ysize-1},
    {COL8_FFFFFF,        3, ysize-24,      59, ysize-24},
    {COL8_000000,        2, ysize-24,       2, ysize-4},
    // {COL8_FFFFFF,        2, ysize-24,       2, ysize-4},
    {COL8_848484,        3, ysize-4,       59, ysize-4},
    {COL8_848484,       59, ysize-23,      59, ysize-5},
    {COL8_000000,        2, ysize-3,       59, ysize-3},
    {COL8_000000,       60, ysize-24,      60, ysize-3},
    {COL8_848484, xsize-47, ysize-24, xsize-4, ysize-24},
    {COL8_848484, xsize-47, ysize-23, xsize-47,ysize-4},
    {COL8_FFFFFF, xsize-47, ysize-3,  xsize-4, ysize-3},
    {COL8_FFFFFF, xsize-3,  ysize-24, xsize-3, ysize-3}
  };
  int row;
  // *u 相当于 arr[0] sizeof = 5 * 4 (int) = 20
  
  // row = (int) (sizeof(*u)/sizeof((*u)[0]));
  // row = (int) (sizeof(*u));
  row = (int) (sizeof(u) / sizeof(u[0]));

  // 用 strlen
  printf("length of u: %d\n", row);
  
  return 0;
}

// void cal_size(int ui[][]) {
void cal_size(int (*ui)[5]) {
  printf("sizeof of ui: %d\n", (int) sizeof(*ui));
  printf("length of ui: %d\n", (int) (sizeof(*ui)/sizeof((*ui)[0])));
}

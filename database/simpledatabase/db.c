#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 简单封装保存的状态，来和 getline() 交互
typedef struct
{
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer() 
{
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("DB > "); }

void read_input(InputBuffer* input_buffer) 
{
  // buffer 一开始是 null，geline 要分配足够内存来保存输入的内容
  ssize_t bytes_read = 
    getline(
      &(input_buffer->buffer),
      &(input_buffer->buffer_length),
      stdin
    );
  if(bytes_read <= 0)
  {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read-1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer)
{
  // 释放 getline 申请的内存
  free(input_buffer->buffer);
  free(input_buffer);
}

// 无限循环 infinite loop
int main(int argc, char* argv[])
{
  InputBuffer* input_buffer = new_input_buffer();
  while(1)
  {
    print_prompt();
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) 
    {
      close_input_buffer(input_buffer);
      printf("Good Bye ~\n");
      exit(EXIT_SUCCESS);
    } else {
      printf("Unrecognized commannd '%s'.\n", input_buffer->buffer);
    }
  }
}

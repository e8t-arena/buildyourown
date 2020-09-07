#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // free,

#ifdef _WIN32
// window 没有 readline 库
// fake 一个
static char input[2048];

char* readline(char* prompt)
{
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  // 初始化空间，从 buffer 复制处输入内容，添加 \0 返回
  char* cpy = malloc(strlen(buffer)+1); // \0 占 1 byte
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

// fake add_history

void add_history(char* unused) {}

#else
// macOS 没有 history.h
#include <editline/readline.h>
// #include <editline/history.h>
#endif


int main(int argc, char** argv) 
{
  char name[] = "c-lisp";
  char prompt[10] = "";
  strcpy(prompt, name);
  strcat(prompt, "> ");

  printf("%s Version 0.0.1", name);
  puts("Press Ctrl+c to Exit\n");

  while(1)
  {
    // 显示 prompt 读取输入
    // fputs(prompt, stdout);

    // 读取输入，最多为 2048 bytes
    // fgets(input, 2048, stdin);

    // 显示 prompt 读取输入
    char* input = readline(prompt);

    // 添加输入到 history
    add_history(input);
    
    printf("Echo: %s", input);

    // 释放输出
    free(input);
  }
  
  return 0;
}

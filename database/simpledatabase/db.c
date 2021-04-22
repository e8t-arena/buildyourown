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

typedef enum {
  META_COMMAND_SUCCESS,
  // 用枚举值处理异常，C 不支持异常处理
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT
} StatementType;

typedef struct {
  StatementType type;
} Statement;

// Helper

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

MetaCommandResult do_meta_command(InputBuffer *input_buffer)
{
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    printf("Done. Good Bye ~~~\n");
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement)
{
  // 比较至少前 n 字符 支持 "insert xxx"
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement *statement)
{
  switch (statement->type) {
    case (STATEMENT_INSERT):
     printf("do insert\n"); 
      break;
    case (STATEMENT_SELECT):
     printf("do select\n"); 
      break;
    default:
      printf("default\n");
  }
}


// 无限循环 infinite loop
int main(int argc, char* argv[])
{
  InputBuffer* input_buffer = new_input_buffer();
  while(1)
  {
    print_prompt();
    read_input(input_buffer);

    if (input_buffer->buffer[0] == '.') {
      // 第一个字符是 . 
      switch (do_meta_command(input_buffer)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command: '%s'\n", input_buffer->buffer);
          continue;
      }
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case (PREPARE_SUCCESS):
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
        continue;
    }

    execute_statement(&statement);
    printf("Executed: %s\n", input_buffer->buffer);
  }
}

/*
 * kilo.c
 * Copyright (C) 2021 Peter Lau <superpeterlau@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

/* #include "kilo.h" */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

/*** Helper ***/ 

void panic(const char *s) {
  perror(s);
  exit(1);
}

void check(int ret, char *msg) {
  if(ret == -1) panic(msg);
}

void log_flag(char *name, int flag) {
  printf("%s\n", name);
  for (int i = 31; i >= 0; i--)
    /* std::cout << ((v >> i) & 1); */
    /* printf("v: %d\n", v>>i&1); */
    printf("%d", flag >> i & 1);
  printf("\n");
}

void log_keypress(char c) {
  if (iscntrl(c)) {
    /* printf("%d\n", c); */
    printf("%d\r\n", c);
  } else {
    /* printf("%d ('%c')\n", c, c); */
    printf("%d ('%c')\r\n", c, c);
  }
}

/*** Terminal ***/ 

struct termios term_termios;

void disableRawMode() {
  /* tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_termios); */
  check(tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_termios), "disableRawMode tcsetattr");
}

void enableRawMode() {
  check(tcgetattr(STDIN_FILENO, &term_termios), "tcgetattr");

  atexit(disableRawMode);

  struct termios raw = term_termios;
  log_flag("ECHO", ECHO);
  log_flag("ICANON", ICANON);
  log_flag("ISIG", ISIG);
  log_flag("VTIME", raw.c_cc[VTIME]);

  raw.c_iflag &= ~(BRKINT |INPCK | ISTRIP | IXON | ICRNL);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(IEXTEN | ECHO | ICANON | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  check(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw), "enableRawMode tcsetattr";
}

int main()
{
  enableRawMode();
  while(1) {
    char c='\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) panic("read");
    log_keypress(c);
    if(c=='q') break;
  }


  /* int ret; */
  /* ret = read(STDIN_FILENO, &c, 1); */
  /* while(ret == 1 && c != 'q') { */
  /*   log_keypress(c); */
  /*   ret = read(STDIN_FILENO, &c, 1); */
  /*   printf("read: %d\n", ret); */
  /* } */

  // while (read(STDIN_FILENO, &c, 1) == 1);
  return 0;
}

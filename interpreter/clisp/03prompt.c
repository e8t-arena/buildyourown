#include <stdio.h>

// 作为 buffer 静态变量全局可见
static char input[2048];

int main(int argc, char** argv) {
	puts("cLisp Version 0.0.1");
	puts("Press Ctrl+c to Exit\n");

	while(1) {
		fputs("cLisp > ", stdout);
		fgets(input, 2048, stdin);
		printf("Echo user input > %s", input);
	}

	return 0;
}

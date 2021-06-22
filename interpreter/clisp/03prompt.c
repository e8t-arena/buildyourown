#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
/* no history.h on macOS */
/* #include <editline/history.h> */


// 作为 buffer 静态变量全局可见
/* static char input[2048]; */

int main(int argc, char** argv) {
	puts("cLisp Version 0.0.1");
	puts("Press Ctrl+c to Exit\n");

	while(1) {
		/* fputs("cLisp > ", stdout); */
		/* fgets(input, 2048, stdin); */

		char* input = readline("cLisp > ");
		/* Add input to history */
		add_history(input);

		printf("Echo user input > %s\n", input);
	}

	return 0;
}

#
# Makefile
# Peter Lau, 2021-06-21 10:47
#
# vim:ft=make

all:
	@echo "Compiler kilo"
	$(MAKE) exec

exec: kilo
	./kilo

kilo: kilo.c
	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99

play: play.c
	$(CC) play.c -o play -Wall -Wextra -Wgnu-binary-literal -pedantic -std=c99 

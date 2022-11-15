CC = gcc
CFLAGS = -Wall -Werror -Wextra 


all: cat

cat:
	@${CC} ${CFLAGS} cat.c -o cat

clean:
	@rm -rf cat 

rebuild: clean cat
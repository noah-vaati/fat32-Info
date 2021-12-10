all: fat32

fat32: fat32.c
	clang -lrt -lpthread -Wall -Wpedantic -Wextra -Werror -o fat32 fat32.c
clean:
	rm -f fat32 
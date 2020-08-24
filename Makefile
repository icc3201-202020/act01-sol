LIBS = -lpthread
CFLAGS = -g -Wall -std=c11

all: child main
	chmod +x child main

main: main.c
	gcc -o main main.c $(CFLAGS)

child: child.c
	gcc -o child child.c $(CFLAGS) $(LIBS)

clean:
	rm child main
	rm -rf main.dSYM child.dSYM
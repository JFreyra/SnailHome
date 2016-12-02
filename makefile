all: snailShell.c
	gcc -o snailShell snailShell.c

run: all
	./snailShell

clean:
	rm snailShell
	rm *.txt
	rm *~

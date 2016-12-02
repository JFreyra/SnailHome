all: snailShellCopy.c
	gcc -o snailShell snailShellCopy.c

run: all
	./snailShell

clean:
	rm snailShell
	rm *.txt
	rm *~

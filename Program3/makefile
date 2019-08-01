CFLAGS= -Wall -std=c99

default: smallsh.c
	gcc -o smallsh smallsh.c ${CFLAGS}

test: default
	p3testscript > mytestresults 2>&1

clean:
	@rm -rf smallsh junk junk2
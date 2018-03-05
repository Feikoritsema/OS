CC=cc
CFLAGS=-D_MINIX
all: shell vsm

shell: shell.c
	${CC} ${CFLAGS} -o shell shell.c

vsm: vsm.c
	${CC} ${CFLAGS} -o vsm vsm.c

clean:
	rm -f *~
	rm -f shell
	rm -f vsm

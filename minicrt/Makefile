
CFLAGS=-fno-builtin -nostdlib -fno-stack-protector -m32
IFLAGS=
SOURCE=$(filter-out test.c,$(wildcard *.c))
OBJS=$(subst .c,.o,${SOURCE})

%.o: %.c
	echo `gcc $(CFLAGS) $(IFLAGS) -c $< -o $@`

a: ${OBJS}
	@echo "haha"

static: ${OBJS}
	ar -rs minicrt.a ${OBJS}

test:
	gcc -m32 -c -ggdb -fno-builtin -nostdlib -fno-stack-protector test.c -o test.o
	ld -m elf_i386 -static -e mini_crt_entry entry.o test.o minicrt.a -o test.out

cplus:
	gcc -c ${CFLAGS} entry.c malloc.c stdio.c string.c printf.c atexit.c
	g++ -c -nostdinc++ -fno-rtti -fno-exceptions ${CFLAGS} crtbegin.cc crtend.cc ctor.cc new_delete.cc sysdep.cc iostream.cc sysdep.cc
	ar -rs minicrt.a malloc.o printf.o stdio.o string.o ctor.o atexit.o iostream.o new_delete.o sysdep.o
	g++ -c -nostdinc++ -fno-rtti -fno-exceptions ${CFLAGS} test.cc
    ld -static -e mini_crt_entry entry.o crtbegin.o test.o minicrt.a crtend.o -o test

clean:
	rm -f *.o *.out
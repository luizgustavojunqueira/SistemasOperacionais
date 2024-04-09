all:	hello myhello puts2

puts.o: puts.s
	cc -c -no-pie puts.s 

exit.o: exit.s
	cc -c -no-pie exit.s

libmylibc.a: puts.o exit.o
	ar rc libmylibc.a puts.o exit.o

hello: hello.s libmylibc.a
	cc -no-pie -c hello.s 
	ld hello.o -o hello -L. -lmylibc

myhello: myhello.c
	gcc -Wno-builtin-declaration-mismatch -Wno-implicit-function-declaration -no-pie myhello.c -nostartfiles -nostdlib -L. -lmylibc -o myhello 

puts2: puts2.c
	gcc -Wno-builtin-declaration-mismatch -no-pie puts2.c -nostartfiles -nostdlib -o puts2 

clean:
	rm -f *~ hello myhello *.o *.a a.out puts2


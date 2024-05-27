all:	contextChangeTime libt.a

libt.a:	
	cd ../../lib && make

contextChangeTime: contextChangeTime.c libt.a
	gcc -no-pie -I../../include contextChangeTime.c -L../../lib -lt -lrt -o contextChangeTime

clean:
	rm -f *.o *~ contextChangeTime core

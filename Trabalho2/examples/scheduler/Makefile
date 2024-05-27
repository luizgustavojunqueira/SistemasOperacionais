all:	scheduler libt.a

libt.a:	
	cd ../../lib && make

scheduler: scheduler.c libt.a
	gcc -no-pie -I../../include scheduler.c -L../../lib -lm -lt -lrt -o scheduler

clean:
	rm -f *.o *~ scheduler core

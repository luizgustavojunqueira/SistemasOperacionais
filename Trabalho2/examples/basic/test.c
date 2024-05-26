#include <stdio.h>
#include <unistd.h>
#include <threadu.h>

#include <util.h>

void *f1(void *p)
{

	int i;
	int *one = (int *)p;

	for (i = 0; i < 3; i++)
	{
		printf("f1 will sleep! %d\n", *one);
		sleep(1);
		printf("f1 will yield!\n");
		thread_yield();
	}
	thread_exit(*one);
}

int main()
{

	thread_t thd;
	int param = 1;
	int rv;

	printf("BEGIN: Time stamp: %ld\n", get_timer());
	thread_init();

	printf("f1 will be created!\n");
	thread_create(&thd, f1, &param);

	printf("f1 will be joined!\n");
	thread_join(&thd, &rv);

	printf("Thread finished with exit status %d\n", rv);


	// thread_t thd[6];
	// int params[6], rv[6], i;

	// thread_init();
	// printf("BEGIN: Time stamp: %ld\n", get_timer());
	// for (i = 0; i < 6; i++)
	// {
	// 	params[i] = i;
	// 	thread_create(&thd[i], f1, &params[i]);
	// }

	// for (i = 0; i < 6; i++)
	// {
	// 	thread_join(&thd[i], &rv[i]);
	// 	printf("Thread %d finished with exit status %d\n", i, rv[i]);
	// }
	// printf("END: Time stamp: %ld\n", get_timer());
}

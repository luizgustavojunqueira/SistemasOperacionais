#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <queue.h>
#include <thread.h>

node_t ready_queue;
tcb_t *current_running;

int tid_global = 0;

/*
  TODO:  thread_init: initializes  the  thread library  and creates  a
  thread for  the main function. Returns  0 on success, or  -EINVAL if
  the library has already been initialized.
 */
int thread_init()
{
	queue_init(&ready_queue);
	return 0;
}

// TODO: creates a new thread and inserts in the ready queue.
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg)
{

	printf("%p", arg); 	

	thread = (thread_t *)malloc(sizeof(thread_t));

	tcb_t *tcb = (tcb_t *)malloc(sizeof(tcb_t));
	tcb->arg = arg;
	tcb->status = FIRST_TIME;
	tcb->tid = tid_global++;
	tcb->start_routine = start_routine;

	thread->tcb = tcb;

	node_t *newNode = (node_t *)malloc(sizeof(node_t));
	newNode->thread = thread;

	enqueue(&ready_queue, newNode);
	
	return 0;
}

// TODO: yields the CPU to another thread
int thread_yield()
{
	node_t* temp = dequeue(&ready_queue);
	enqueue(&ready_queue, temp);

	printf("removido %d\n", ((tcb_t*) temp->thread->tcb)->tid);
	
	// printf("Na lista: \n");
	// print_queue(&ready_queue);
	return 0;
}

// TODO: waits for a thread to finish
int thread_join(thread_t *thread, int *retval)
{
	tcb_t *tcb = (tcb_t*) thread->tcb;
	tcb->start_routine(tcb->arg);
	return 0;
}

// TODO: marks a thread as EXITED and terminates the thread
void thread_exit(int status)
{
}

// TODO: selects the next thread to execute
void scheduler()
{
}

// TODO: you must  make sure this function is called  if a thread does
// not call thread_exit
void exit_handler()
{
}

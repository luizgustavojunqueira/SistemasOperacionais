#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <thread.h>
#include <util.h>

queue_t ready_queue; // Queue containing the ready threads
tcb_t *current_running; // Thread executing
uint64_t current_start_time; // Time when the thread started

int tid_global = 0;

/*
  TODO:  thread_init: initializes  the  thread library  and creates  a
  thread for  the main function. Returns  0 on success, or  -EINVAL if
  the library has already been initialized.
 */
int thread_init()
{

	if(current_running != NULL)
	{
		return -EINVAL;
	}

	// Initialize the ready queue
	queue_init(&ready_queue); 

	// Create a tcb for the main thread
	tcb_t *main_tcb = (tcb_t *)malloc(sizeof(tcb_t));

	// Set the main thread as the current running thread
	current_running = main_tcb;

	// Set the status of the main thread as FIRST_TIME
	main_tcb->status = FIRST_TIME;

	// Set the tid of the main thread as 0
	main_tcb->tid = tid_global++;

	current_start_time = get_timer();

	return 0;
}

// TODO: creates a new thread and inserts in the ready queue.
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg)
{

	// Allocate memory for the new tcb
	tcb_t *new_tcb = (tcb_t *)malloc(sizeof(tcb_t));

	// Set the status of the new tcb as FIRST_TIME
	new_tcb->status = FIRST_TIME;

	// Set the tid of the new tcb
	new_tcb->tid = tid_global++;

	// Set the start_routine of the new tcb
	new_tcb->start_routine = start_routine;

	// Set the cpu_time of the new tcb
	new_tcb->cpu_time = 0;

	// Put exit_handler() address in the last position of the stack on the tcb
	new_tcb->stack[STACK_SIZE-1] = (uint64_t*)&exit_handler;

	// Set the rsp of the new tcb to the last position of the stack
	new_tcb->rsp = &new_tcb->stack[STACK_SIZE-1];

	// Set the argument for the start_routine on reg[9] = %rdi
	new_tcb->regs[9] = (uint64_t)arg;

	// Assign the tcb to the thread
	thread->tcb = new_tcb;

	// Create a new node for the tcb to be inserted in the ready queue
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	new_node->tcb = new_tcb;

	// Insert the tcb in the ready queue
	enqueue(&ready_queue, new_node, SCHEDULER_TYPE);

	return 0;
}

// TODO: yields the CPU to another thread
int thread_yield()
{

	// Calculate the cpu time of the current running thread
	current_running->cpu_time += get_timer() - current_start_time;

	// Set the status of the current running thread as READY
	current_running->status = READY;

	// Create a new node for the current running thread to be inserted in the ready queue
	node_t *node = (node_t *)malloc(sizeof(node_t));
	node->tcb = current_running;

	// Insert the current running thread in the ready queue
	enqueue(&ready_queue, node, SCHEDULER_TYPE);

	// Call the scheduler to select the next thread to execute and change context
	scheduler_entry();

	return 0;
}

// TODO: waits for a thread to finish
int thread_join(thread_t *thread, int *retval)
{
	// Get the tcb of the thread
	tcb_t *tcb = thread->tcb;

	// Wait for the thread to finish
	while (tcb->status != EXITED)
	{
		thread_yield();
	}

	// Set the return value of the thread
	if(retval != NULL)
	{
		// The return value is in reg[14] = %rax
		*retval = tcb->regs[14];
	}

	return 0;
}

// TODO: marks a thread as EXITED and terminates the thread
void thread_exit(int status)
{

	// Mark the current running thread as EXITED
	current_running->status = EXITED;

	scheduler_entry();
}

// TODO: selects the next thread to execute
void scheduler()
{

	// Get the next thread to execute
	node_t *node = dequeue(&ready_queue);
	
	// Set the next thread to execute as the current running thread
	current_running = node->tcb;

	// Set the status of the next thread to RUNNING
	current_running->status = RUNNING;

	// Set the start time of the next thread
	current_start_time = get_timer();

	return;
}

// TODO: you must  make sure this function is called  if a thread does
// not call thread_exit
void exit_handler(void *arg)
{
	// Call the start_routine of the thread
	current_running->start_routine(arg);
	// If the thread does not call thread_exit, call it
	thread_exit(0);
}
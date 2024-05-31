#include <lock.h>
#include <stdlib.h>
#include <stdio.h>

#include <thread.h>

enum
{
	SPIN = FALSE,
};

// TODO: inicializes a lock
void lock_init(lock_t *l)
{
	if (SPIN)
	{
		
		l->status = UNLOCKED;
	}
	else
	{
		// Initialize the blocked queue for the lock
		queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
		queue_init(queue);

		// Initialize the lock as unlocked
		l->queue = queue;
		l->status = UNLOCKED;
	}
}

// TODO:  acquires a  lock if  it is  available or  blocks the  thread
// otherwise
void lock_acquire(lock_t *l)
{
	if (SPIN)
	{
		while (LOCKED == l->status)
			thread_yield();
		l->status = LOCKED;
	}
	else
	{
		// If the lock is not initialized, initialize it
		if(l->queue == NULL)
		{
			lock_init(l);
		}

		// If the lock is already acquired by another thread, block the thread
		if (LOCKED == l->status)
		{
			block(l);
		}
		l->status = LOCKED;
	}
}

// TODO:  releases a  lock  and  unlocks one  thread  from the  lock's
// blocking list
void lock_release(lock_t *l)
{
	if (SPIN)
	{
		l->status = UNLOCKED;
	}
	else
	{

		// If theres no blocked threads in the lock, unlock the lock
		if (l->queue->head == NULL)
		{
			l->status = UNLOCKED;
		}
		else // If there are blocked threads, unblock one of them
		{
			unblock(l);
		}
	}
}

// TODO: blocks the running thread
void block(lock_t *l)
{
	// Create a new node for the blocked thread
	node_t *newNode = (node_t *)malloc(sizeof(node_t));
	newNode->tcb = current_running;

	// Add the blocked thread to the lock's queue
	enqueue(l->queue, newNode, 0);

	// Change the status of the blocked thread to BLOCKED
	current_running->status = BLOCKED;

	// Call the scheduler to choose a new thread to run
	scheduler_entry();
}

// TODO: unblocks  a thread that is waiting on a lock.
void unblock(lock_t *l)
{
	// Unblock the first thread in the lock's queue
	node_t *releaseThread = dequeue(l->queue);

	// Change the status of the unblocked thread to READY
	((tcb_t *)releaseThread->tcb)->status = READY;

	// Add the unblocked thread to the ready queue
	enqueue(&ready_queue, releaseThread, SCHEDULER_TYPE);
}

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
		queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
		queue_init(queue);

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
		if (l->queue->head == NULL)
		{
			l->status = UNLOCKED;
		}
		else
		{
			unblock(l);
		}
	}
}

// TODO: blocks the running thread
void block(lock_t *l)
{
	node_t *newNode = (node_t *)malloc(sizeof(node_t));
	newNode->tcb = current_running;
	enqueue(l->queue, newNode, 0);
	current_running->status = BLOCKED;
	scheduler_entry();
}

// TODO: unblocks  a thread that is waiting on a lock.
void unblock(lock_t *l)
{
	node_t *releaseThread = dequeue(l->queue);
	enqueue(&ready_queue, releaseThread, SCHEDULER_TYPE);
	((tcb_t *)releaseThread->tcb)->status = READY;
}

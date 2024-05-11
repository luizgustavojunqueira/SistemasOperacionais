#include <queue.h>
#include <thread.h>
/*
  This   file    contains   a    function   definitions    for   queue
  manipulation. You  are free to  choose your own  implementation. You
  may need to define additional functions if you want to implement the
  extra-point functionality.
 */
// inicializes a queue with head
node_t *first, *last;
void queue_init(node_t * queue)
{
  queue = (node_t *)malloc(sizeof(node_t));
  queue->next=NULL;
  first=queue->next;
  last=first;

}

// TODO: returns the first element of the queue
node_t *dequeue(node_t * queue)
{
  
	return queue->next;
}

// inserts a node in a queue
void enqueue(node_t * queue, node_t * item)
{
  node_t *newNode;
  newNode = (node_t *) malloc(sizeof (node_t));
  newNode->tcb_t = item;
  newNode->next = queue->next;
  queue->next = newNode;

}

// TODO: checks if a queue is empty
int is_empty(node_t *queue)
{
	if(queue->next==NULL)
  {
    return 1;
  }
  return 0;
}




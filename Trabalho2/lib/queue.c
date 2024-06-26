#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
/*
  This   file    contains   a    function   definitions    for   queue
  manipulation. You  are free to  choose your own  implementation. You
  may need to define additional functions if you want to implement the
  extra-point functionality.
 */
// iInitializes a queue with head
void queue_init(queue_t *queue)
{
  queue->head = NULL;
  queue->tail = NULL;
}

// TODO: returns the first element of the queue
node_t *dequeue(queue_t *queue)
{
  // If the queue is empty, return NULL
  if (queue->head == NULL)
  {
    return NULL;
  }

  // Remove the first element of the queue
  node_t *temp = queue->head;
  queue->head = queue->head->next;

  // If the queue is empty after removing, set the tail to NULL
  if (queue->head == NULL)
  {
    queue->tail = NULL;
  }

  return temp;
}

// inserts a node in a queue
void enqueue(queue_t *queue, node_t *item, int schedulerType)
{
  // If the scheduler type is TRUE, insert the node in the queue sorted by cpu_time
  if (schedulerType == TRUE)
  {
    enqueue_sort(queue, item, lessthan);
    return;
  }

  // If the scheduler type is FALSE, insert the node in the queue in FIFO order

  // If the queue is empty, set the head and tail to the new node
  if (queue->head == NULL)
  {
    queue->head = item;
    queue->tail = item;
    return;
  }

  // If the queue is not empty, insert the new node in the tail
  queue->tail->next = item;
  queue->tail = item;

  item->next = NULL;
}

// returns the first element of the queue
node_t *peek(queue_t *queue)
{
  return queue->head;
}

// TODO: checks if a queue is empty
int is_empty(queue_t *queue)
{
  // If the head is NULL, the queue is empty
  return queue->head == NULL;
}

int lessthan(node_t *a, node_t *b)
{
  // If the nodes are NULL, return 0
  if (a == NULL || b == NULL)
  {
    return 0;
  }

  // Get the tcb from each node
  tcb_t *tcb_a = (tcb_t *)a->tcb;
  tcb_t *tcb_b = (tcb_t *)b->tcb;

  // Return true if the cpu_time of tcb_b is less than tcb_a
  return tcb_a->cpu_time > tcb_b->cpu_time;
}

void enqueue_sort(queue_t *q, node_t *item, node_lte comp)
{

  node_t *current = q->head;
  node_t *prev = NULL;

  // Iterate over the queue until it finds a node that is greater than the item
  while (current != NULL && comp(item, current))
  {
    prev = current;
    current = current->next;
  }

  // If the queue is empty, or the item is the lowest, insert on the head
  if (prev == NULL)
  {
    item->next = q->head;
    q->head = item;
  }
  else // Insert the item in the middle of the queue
  {
    prev->next = item;
    item->next = current;
  }

  if (current == NULL) // If the item is the highest, insert on the tail
  {
    q->tail = item;
  }
}
#include <queue.h>
#include <stdio.h>
#include <stdlib.h>
/*
  This   file    contains   a    function   definitions    for   queue
  manipulation. You  are free to  choose your own  implementation. You
  may need to define additional functions if you want to implement the
  extra-point functionality.
 */
// initializes a queue with head
void queue_init(queue_t *queue)
{
  queue->head = NULL;
  queue->tail = NULL;
}

// TODO: returns the first element of the queue
node_t *dequeue(queue_t *queue)
{

  if (queue->head == NULL)
  {
    return NULL;
  }

  node_t *temp = queue->head;
  queue->head = queue->head->next;

  if (queue->head == NULL)
  {
    queue->tail = NULL;
  }

  return temp;
}

// inserts a node in a queue
void enqueue(queue_t *queue, node_t *item)
{

  if (queue->head == NULL)
  {
    queue->head = item;
    queue->tail = item;
    return;
  }

  queue->tail->next = item;

  queue->tail = item;

  item->next = NULL;

  return;
}

// returns the first element of the queue
node_t *peek(queue_t *queue)
{
  return queue->head;
}

// TODO: checks if a queue is empty
int is_empty(queue_t *queue)
{
  return queue->head == NULL;
}
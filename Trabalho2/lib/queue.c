#include <queue.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
/*
  This   file    contains   a    function   definitions    for   queue
  manipulation. You  are free to  choose your own  implementation. You
  may need to define additional functions if you want to implement the
  extra-point functionality.
 */
// inicializes a queue with head
node_t *first, *last;
void queue_init(node_t *queue)
{
  queue = (node_t *)malloc(sizeof(node_t));
  queue->next = NULL;
  first = queue->next;
  last = first;
  printf("init queue\n");
}

// TODO: returns the first element of the queue
node_t *dequeue(node_t *queue)
{
  node_t *temp;
  temp = queue->next;
  queue->next = temp->next;
  temp->next = NULL;

  // printf("dequeuing %d\n", ((tcb_t* )temp->thread->tcb)->tid);

  return temp;
}

// inserts a node in a queue
void enqueue(node_t *queue, node_t *item)
{
  node_t *p = queue->next;

  if (p == NULL)
  {
    queue->next = item;
    first = queue->next;
    last = first;
    return;
  }

  while (p->next != NULL)
  {
    p = p->next;
  }
  p->next = item;
  last = p->next;
}

// returns the first element of the queue
node_t *peek(node_t *queue)
{
  return queue->next;
}

// TODO: checks if a queue is empty
int is_empty(node_t *queue)
{
  if (queue->next == NULL)
  {
    return 1;
  }
  return 0;
}

void print_queue(node_t *queue)
{
  node_t *p = queue->next;
  while (p->next != NULL)
  {
    printf("%d\n", ((tcb_t *)p->thread->tcb)->tid);
    p = p->next;
  }

  printf("%d\n", ((tcb_t *)p->thread->tcb)->tid);
}
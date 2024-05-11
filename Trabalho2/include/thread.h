#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <threadu.h>

#define NUMBER_OF_REGISTERS	15
#define STACK_SIZE		2048

void scheduler_entry();
void exit_handler();

typedef enum {
	      FIRST_TIME,
	      READY,
	      BLOCKED,
	      EXITED
} status_t;

typedef struct tcb {
	int tid;
	uint64_t rgs[NUMBER_OF_REGISTERS];
	uint64_t stack[STACK_SIZE];
	status_t status;
} tcb_t;

#endif /* THREAD_H */

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
	uint64_t rgs[NUMBER_OF_REGISTERS];
	uint64_t stack[STACK_SIZE];
	int tid;
	status_t status;
	void *(*start_routine)(void*);// ponteiro para as função que a thread executa
	void *arg;//argumentos da função
	void *retval;//retorno da função
} tcb_t;

#endif /* THREAD_H */

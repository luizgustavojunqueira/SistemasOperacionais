#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <threadu.h>
#include <queue.h>

#define NUMBER_OF_REGISTERS 15
#define STACK_SIZE 2048

void scheduler_entry();
void exit_handler();

/*
	Escolha do tipo de escalonador
	FALSE = FIFO
	TRUE = Escolher a tarefa com o menor tempo de cpu.
*/
enum
{
	SCHEDULER_TYPE = FALSE
};

typedef enum
{
	FIRST_TIME,
	READY,
	BLOCKED,
	RUNNING,
	EXITED
} status_t;

typedef struct tcb
{
	uint64_t flags;						// registrador de flags
	uint64_t regs[NUMBER_OF_REGISTERS]; // registradores
	void *rsp;							// topo da pilha
	uint64_t *stack[STACK_SIZE];		// pilha

	status_t status;				
	int tid;						
	void *(*start_routine)(void *); 
	uint64_t cpu_time;				
	uint64_t lixo;					

} tcb_t;

// Define the ready queue and current running thread variables
extern queue_t ready_queue;
extern tcb_t *current_running;

#endif /* THREAD_H */

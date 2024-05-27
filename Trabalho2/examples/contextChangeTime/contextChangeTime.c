#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <threadu.h>
#include <inttypes.h>
#include "util.h"

uint64_t tempo_saida;

void *f1(){
    tempo_saida = get_timer();
    thread_exit(0);
}

int main(){

    thread_init();

    uint64_t tempo_entrada;
    thread_t *thd = (thread_t *)malloc(sizeof(thread_t));

    thread_create(thd, f1, NULL);

    tempo_entrada = get_timer();
    thread_yield();

    print_str(1, 1, "CPU cycles taken to change context: ");
    print_int(1, 36, tempo_saida - tempo_entrada);
    print_str(1, 39, " cycles");
}

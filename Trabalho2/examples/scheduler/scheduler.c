#include <stdio.h>
#include <unistd.h>
#include <threadu.h>

#include <util.h>
#include <math.h>

/*
    Programa usado para testar o escalonador de threads por tempo de cpu

    São 3 threads competindo por tempo de CPU, e 3 funções diferentes (f1,f2,f3)

    As funções fazem todas a mesma coisa, a unica diferença é o tamanho dos laços internos

    f1 chama yield 50 vezes no laço externo
    f2 chama yield 20 vezes no laço externo
    f3 chama yield 5 vezes no laço externo

    O laço interno de f1 é executado 10000 vezes
    O laço interno de f2 é executado 100000 vezes
    O laço interno de f3 é executado 1000000 vezes

    Usando o escalonador por tempo de cpu, f1 vai ser escalonado com maior frequencia, f2 com menos frequencia que 1
    e f3 com a menor frequencia de todas.
*/

void *f1()
{
    double p;
    for (int j = 0; j < 50; j++)
    {
        printf("F1 %d outer loop iteration\n", j + 1);
        for (int i = 0; i < 10000; i++)
        {
            p += pow(j, i);
        }

        thread_yield();
    }
}

void *f2()
{
    double p;
    for (int j = 0; j < 20; j++)
    {
        printf("F2 %d outer loop iteration\n", j + 1);
        for (int i = 0; i < 100000; i++)
        {
            p += pow(j, i);
        }

        thread_yield();
    }
}

void *f3()
{
    double p;
    for (int j = 0; j < 5; j++)
    {
        printf("F3 %d outer loop iteration\n", j + 1);
        for (int i = 0; i < 1000000; i++)
        {
            p += pow(j, i);
        }

        thread_yield();
    }
}

int main()
{

    thread_t thd[3];

    thread_init();

    printf("BEGIN: Time stamp: %ld\n", get_timer());

    thread_create(&thd[0], f1, NULL);
    thread_create(&thd[1], f2, NULL);
    thread_create(&thd[2], f3, NULL);

    for (int i = 0; i < 3; i++)
    {
        thread_join(&thd[i], NULL);
        printf("Thread %d finished\n", i + 1);
    }

    printf("END: Time stamp: %ld\n", get_timer());
}
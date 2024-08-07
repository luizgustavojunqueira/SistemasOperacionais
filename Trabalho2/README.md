# Trabalho 2

## Objetivo

O objetivo principal deste trabalho é criar uma biblioteca de threads em nível de usuário para suportar multiprogramação.
Mais especificamente a implementação das seguintes funcionalidades:

- Escalonador não preemptivo
- Troca de contexto entre threads
- Primitivas básicas de exclusão mútua
- Medir custo de trocas de contexto

### Ponto Extra

Para um ponto extra no trabalho, foi implementado um escalonador justo, que escolhe a próxima tarefa a executar com base no tempo de CPU até o momento da escolha.

Para mudar o escalonador usado, mude a variável SCHEDULER_TYPE definida no arquivo thread.h

TRUE = Escalonador Justo (por tempo de CPU)
FALSE = Round Robin (FCFS)

# Lab 2 - Rendiment


## Taula de l'apartat 1

Temps obtingut a partir de 10 milions d'execucions de cada funció i fer la mitja aritmètica

| Syscall                        | sbrk(0) | sbrk(inc) | sched_yield() | getpid() | fork/waitpid |
|--------------------------------|---------|-----------|---------------|----------|--------------|
| Execution time (in microsecs.) |         |           |               |          |              |


Podem observar clarament com el temps del sbrk amb increment és major que el de la mateixa
funció però sense increment. Això és degut a que sense increment només s'ha retornar el valor del
SP.

Per altra banda, dir que el temps del sched_yield és tan elevat degut a que s'ha de cambiar de procés
(potencialment), i això és una operació molt costosa.

Podem veure també com el getpid és bastant similar al sbrk sense increment, ja que només ha de
retornar un valor del PCB.

El fork i el waitpid té un cos bastant inferior al sched_yield ja que, encara que es fa un canvi de
context, no s'ha d'invalidar el TLB ja que l'espai d'adreces serà el mateix degut a les polítiques de
reemplaçament de pàgines del sistema operatiu Linux. Però, en el cas d'haver de crear un nou espai
d'adreces pel fill, llavors el cost hauria de ser major que pel sched_yield.


## Gràfiques de l'apartat 2

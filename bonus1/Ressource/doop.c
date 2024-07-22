#include <stdio.h>
#include <stdlib.h>

int main(int ac, char **av){
    int v5 = atoi(av[1]);
    printf("*4: %zu\n", (size_t)(4 * v5));
    printf("*1: %zu\n", (size_t)(v5));
    printf("/4: %zu\n", ((size_t)(v5))/4);
}
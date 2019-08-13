#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[])
{
    char * key;
    int size, i;

    //ensure there are two arguments
    //this does not do type checking, 
    //so nonnumeric inputs cause undefined behaviour
    if(argc != 2){
        printf("Usage: keygen <size of key>\n");
        exit(1);
    }

    //seed random number generator
    int seed = time(NULL);
    srand(seed);

    size = atoi(argv[1]);
    key = malloc(size);

    for(i = 0; i < size; i++){
        key[i] = rand() % 27 + 65; //65 is ASCII 'A'
        if(key[i] == 91){
            key[i] = 32; //32 is ASCII ' '
        }
    }
    printf("%s\n", key);

    free(key);
    return 0;
}
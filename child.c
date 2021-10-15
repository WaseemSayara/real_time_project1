#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{


    printf("0 = %s , 1 = %s \n", argv[0], argv[1]);
    int x = atoi(argv[0]);

    int i;

    long t = time(NULL) * x;
    printf("time from chid %d = %ld\n",x, t);
    srand(t);
    for (i = 0 ; i<10; i++){
        printf("random %d from child %d = %d\n",i,x, (rand() % 100) + 1);

    }


    return 0;
}
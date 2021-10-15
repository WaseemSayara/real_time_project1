

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int i, childNumber;
    long t;
    FILE *ptr;

    childNumber = atoi(argv[0]);

    t = time(NULL) * childNumber;
    printf("time from chid %d = %ld\n", childNumber, t);
    srand(t);

    char filename[30] = "./child";
    strcat(filename, argv[0]);
    strcat(filename, ".txt");
    printf("%s\n", filename);

    ptr = fopen(filename, "w+");

    for (i = 0; i < 10; i++)
    {
        fprintf(ptr, "%d\n", (rand() % 100) + 1);
    }
    
    kill(getppid(), SIGINT);

    return 0;
}
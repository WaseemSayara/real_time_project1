

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

    printf("im chid %d = %d\n", childNumber, getpid());
    fflush(stdout);

    childNumber = atoi(argv[0]);

    t = time(NULL) * childNumber;
    printf("time from chid %d = %ld\n", childNumber, t);
    fflush(stdout);
    srand(t);

    char filename[30] = "./child";
    strcat(filename, argv[0]);
    strcat(filename, ".txt");
    ptr = fopen(filename, "w+");

    for (i = 0; i < 10; i++)
    {
        fprintf(ptr, "%d\n", (rand() % 100) + 1);
    }

    close(ptr);
    sleep(childNumber);
    kill(getppid(), SIGINT);

    return 0;
}
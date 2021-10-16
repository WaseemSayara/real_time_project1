

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

void signal_catcher(int);

int main(int argc, char *argv[])
{

    if (sigset(SIGUSR1, signal_catcher) == SIG_ERR)
    {
        perror("Sigset can not set SIGUSR1");
        exit(SIGUSR1);
    }
    int i, childNumber;
    long t;
    FILE *ptr;
    t = time(NULL) * childNumber;
    srand(t);

    printf("hello from child %d\n", childNumber);

    while (1)
    {
        pause();
        childNumber = atoi(argv[0]);
        char filename[30] = "./child";
        strcat(filename, argv[0]);
        strcat(filename, ".txt");
        ptr = fopen(filename, "w+");

        for (i = 0; i < 10; i++)
        {
            fprintf(ptr, "%d\n", (rand() % 100) + 1);
        }

        fclose(ptr);
        // sleep(childNumber * 2);
        kill(getppid(), SIGINT);
    }
    return 0;
}

void signal_catcher(int the_sig)
{
    printf("SIGUSR1 received (child %d start)\n", getpid());
    fflush(stdout);
}
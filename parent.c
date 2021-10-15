
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

void signal_catcher1(int);

int main(int argc, char *argv[])
{

    int i, status;
    pid_t pid, pid_array[3];

    if (sigset(SIGINT, signal_catcher1) == SIG_ERR)
    {
        perror("Sigset can not set SIGQUIT");
        exit(SIGQUIT);
    }

    printf("My process ID is %d\n", getpid());

    for (i = 0; i < 3; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            perror("failed to fork childs");
            exit(-1);
        }
        else if (pid == 0)
        {
            if (i == 0)
            {
                printf(" referee %d forked, with id = %d\n", i, getpid());
                fflush(stdout);
                pause();
            }
            else
            {
                char snum[5];
                sprintf(snum, "%d", i);
                int x = execl("./child", snum, (char *)0);
                printf("%d", x);
                fflush(stdout);
                pause();
            }
        }
        else
        {
            pid_array[i] = pid;
        }
    }

    pause();

    return 0;
}

void signal_catcher1(int the_sig)
{
    printf("Youpiiiiiiiii!\n");
}

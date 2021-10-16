
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

void signal_catcher_children(int);
void signal_catcher(int);

int ready = 0;

int main(int argc, char *argv[])
{

    int i, status;
    pid_t pid, pid_array[3];

    if (sigset(SIGINT, signal_catcher_children) == SIG_ERR)
    {
        perror("Sigset can not set SIGINT");
        exit(SIGINT);
    }

    if (sigset(SIGQUIT, signal_catcher) == SIG_ERR)
    {
        perror("Sigset can not set SIGQUIT");
        exit(SIGQUIT);
    }

    printf("My process ID is %d\n", getpid());
    fflush(stdout);

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
                int x = execl("./referee", NULL, (char *)0);
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

    while ( 1 )
    {
        if ( ready == 2 ){
            ready = 0;
            printf(" lets go to referee\n");
            fflush(stdout);
            break;
        }
        else{
            pause();
        }
    }
    

    for (i = 0; i < 3; i++)
    {
        if (waitpid(pid_array[i], &status, 0) == pid_array[i])
        {
            printf("Process ID %d has terminated\t status = %d\n", pid_array[i], status);
            fflush(stdout);
        }
    }
    return 0;
}

void signal_catcher_children(int the_sig)
{
    printf("SIGINT received\n");
    fflush(stdout);

    ready++;

    if (the_sig == SIGQUIT)
        exit(1);
}

void signal_catcher(int the_sig)
{
    printf("SIGQUIT received\n");
    fflush(stdout);

    if (the_sig == SIGQUIT)
        exit(1);
}

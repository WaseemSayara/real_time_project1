
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

void signal_catcher_children(int);
void signal_catcher(int);

int ready = 0;
int BigScore1 = 0;
int BigScore2 = 0;
int main(int argc, char *argv[])
{

    int i, status;
    pid_t pid, pid_array[3];
    int f_des[2];
    static char message[BUFSIZ];

    if (pipe(f_des) == -1)
    {
        perror("Pipe");
        exit(2);
    }

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
                char pipe_read[5], pipe_write[5];
                sprintf(pipe_read, "%d", f_des[0]);
                sprintf(pipe_write, "%d", f_des[1]);
                int x = execl("./referee", pipe_read, pipe_write, (char *)0);
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

    while (BigScore1 < 50 && BigScore2 < 50)
    {
        sleep(1);
        kill(pid_array[1], SIGUSR1);
        kill(pid_array[2], SIGUSR1);
        if (ready == 2)
        {
            ready = 0;
            printf(" lets go to referee\n");
            fflush(stdout);
            char filenames[] = "child1.txt-child2.txt";
            if (write(f_des[1], filenames, strlen(filenames)) != -1)
            {
                printf("Message sent by parent: [%s]\n", filenames);
                fflush(stdout);
                sleep(1);
                if (read(f_des[0], message, BUFSIZ) != -1)
                {
                    printf(message);
                    char *token = strtok(message, "-");
                    int score1 = atoi(token);
                    token = strtok(NULL, "-");
                    int score2 = atoi(token);

                    if (score1 > score2)
                        BigScore1++;
                    else if (score1 < score2)
                        BigScore2++;
                    else
                    {
                        BigScore1++;
                        BigScore2++;
                    }
                    printf("BigScore1= %d, BigScore2= %d\n", BigScore1, BigScore2);
                }
            }
            else
            {
                perror("Write");
                exit(5);
            }
        }
        else
        {
            pause();
        }
    }

    if (BigScore1 > BigScore2)
        printf("Child 1 Wins!\n");
    else if (BigScore1 < BigScore2)
        printf("Child 2 Wins!\n");
    else
        printf("Draw!\n");

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

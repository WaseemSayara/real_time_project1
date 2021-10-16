
/*
* The Parent code
*/

#include "local.h"
#define WIN_SCORE 3

//System calls for functions used to work with signals
void signal_catcher_children(int);
void signal_catcher(int);

//initialize variables
int ready = 0, BigScore1 = 0, BigScore2 = 0, roundNum = 0;
pid_t pid, pid_array[3];

int main(int argc, char *argv[])
{

    int i, status, f_des[2];     // f-des exprsee read(0) & write(1) operations
    pid_t pid, pid_array[3];     // pid_array to store childern IDs
    static char message[BUFSIZ]; //message used in piping process with size equal to 8192

    //Pipe exception with perror message
    if (pipe(f_des) == -1)
    {
        perror("Pipe Error!");
        exit(2);
    }

    /*
    * Identify signal catchcers for each of SIGINT and SIGQUIT where the parent is sensitive to them 
    */
    if (sigset(SIGINT, signal_catcher_children) == SIG_ERR)
    {
        perror("Sigset can not set SIGINT!");
        exit(SIGINT);
    }

    if (sigset(SIGQUIT, signal_catcher) == SIG_ERR)
    {
        perror("Sigset can not set SIGQUIT!");
        exit(SIGQUIT);
    }

    /*
    * Fork Referee and two children processes
    */
    for (i = 0; i < 3; i++)
    {
        pid = fork(); //store forked proceee pid in pid variable

        // Failed fork process
        if (pid == -1)
        {
            perror("failed to fork childs");
            exit(-1);
        }
        // The process is a child
        else if (pid == 0)
        {
            // Assign Referee process
            if (i == 0)
            {
                char pipe_read[5], pipe_write[5];

                // Convert from integer to String to send in argv
                sprintf(pipe_read, "%d", f_des[0]);
                sprintf(pipe_write, "%d", f_des[1]);

                int status = execl("./referee", pipe_read, pipe_write, (char *)0);

                if (status == -1)
                    perror("Faild to execute ./referee!");

               
            }

            // Assign children processes
            else
            {
                char child_num[3]; // Used to identified children ( 1 or 2)
                sprintf(child_num, "%d", i);
                int status = execl("./child", child_num, (char *)0);

                if (status == -1)
                    perror("Faild to execute ./child!");

                
            }
        }

        // Parent Case
        else
        {
            pid_array[i] = pid; // save children pids
        }
    }

    /*
    * Compare BigScore for each child after every round 
    */

    //Keep going until at least one of the 2 children score reach 50
    while (BigScore1 < WIN_SCORE && BigScore2 < WIN_SCORE)
    {
        sleep(1);

        //Kill system call using SIGUSR1 signal to tell children to start work
        kill(pid_array[1], SIGUSR1);
        kill(pid_array[2], SIGUSR1);

        pause(); // waite until the two children end there works

        if (ready == 2)
        {
            ready = 0; // reset counter which is responsible on ready state of the two children

            // variable express name of the two text files that include random numbers
            char filenames[] = "child1.txt-child2.txt";

            /*
            * send names of the text files from parent to referee
            */
            if (write(f_des[1], filenames, strlen(filenames)) != -1)
            {

                sleep(1);

                if (read(f_des[0], message, BUFSIZ) != -1)
                {

                    char *token = strtok(message, "-");
                    int score1 = atoi(token);
                    token = strtok(NULL, "-");
                    int score2 = atoi(token);
                    roundNum++;
                    reset();
                    printf("_______________________________________________\n");
                    yellow();
                    printf("\n\t      ### Round ( %d ) ###        \n", roundNum);
                    reset();
                    printf("\n\tP1 score = %d   |    P2 score= %d\n\n", score1, score2);
                    fflush(stdout); // To be sure that message will not ignored

                    // Modify children scores in every round
                    if (score1 > score2)
                        BigScore1++;
                    else if (score1 < score2)
                        BigScore2++;
                    // Equality case
                    else
                    {
                        BigScore1++;
                        BigScore2++;
                    }
                    //Show Bigscore at the end of each round
                    cyan();
                    printf("\tBigScore1= %d   |   BigScore2= %d\n", BigScore1, BigScore2);
                    fflush(stdout);
                }
            }
            // Failed write process
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

    /*
    * Compare BigScore for each child after at least one of them reach 50 to show the final result
    */
    if (BigScore1 > BigScore2)
    {
        green();
        printf("\n================   P1 Wins!    =================n\n");
    }
    else if (BigScore1 < BigScore2)
    {
        green();
        printf("\n================   P2 Wins!    ================\n\n");
    }
    else
    {
        green();
        printf("\n================     Draw!     ================\n\n");
    }
    // Kill all children after end the game
    for (int i = 0; i < 3; i++)
        kill(SIGKILL, pid_array[i]);

    return 0;
}

/*
* Children SIGINT signals catcher
* tell parent that the 2 children end there work and fill text files
*/

void signal_catcher_children(int the_sig)
{
    ready = 2;

    if (the_sig == SIGQUIT)
        exit(1);
}

/*
 * SIGQUIT signal catcher which it will kill all children then kill the parent
*/
void signal_catcher(int the_sig)
{
    for (int i = 0; i < 3; i++)
        kill(SIGKILL, pid_array[i]);

    kill(SIGKILL, getpid()); // To kill parent

    if (the_sig == SIGQUIT)
        exit(1);
}

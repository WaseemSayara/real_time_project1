
/*
* The Parent code
*/

#include "local.h"
#define WIN_SCORE 50

// System calls for functions used to work with signals
void signal_catcher_children(int);
void signal_catcher(int);

// Initialize variables
int ready = 0, BigScore1 = 0, BigScore2 = 0, roundNum = 0;
pid_t pid, pid_array[3];

int main(int argc, char *argv[])
{
    /*
    * f-des exprsee read(0) & write(1) operations
    * pid_array to store childern IDs
    * message used in piping process with size equal to 8192
    */
    int i, status, f_des[2];
    pid_t pid, pid_array[3];
    static char message[BUFSIZ];

    // Pipe exception with perror message
    if (pipe(f_des) == -1)
    {
        perror("Pipe Error!");
        exit(1);
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
            exit(2);
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
                {
                    perror("Faild to execute ./referee!");
                    exit(3);
                }
            }

            // Assign children processes
            else
            {
                // Used to identified children ( 1 or 2)
                char child_num[3];
                sprintf(child_num, "%d", i);
                int status = execl("./child", child_num, (char *)0);

                if (status == -1)
                {
                    perror("Faild to execute ./child!");
                    exit(4);
                }
            }
        }

        // Parent Case
        else
        {
            // save children pids
            pid_array[i] = pid;
        }
    }

    /*
    * Compare BigScore for each child after every round 
    */

    //Keep going until at least one of the 2 children score reach 50
    while (BigScore1 < WIN_SCORE && BigScore2 < WIN_SCORE)
    {
        // to give the childen enough time to do thier work
        sleep(1);

        // none of the childs is working case
        if (ready == 0)
        {
            //Kill system call using SIGUSR1 signal to tell children to start work
            kill(pid_array[1], SIGUSR1);
            kill(pid_array[2], SIGUSR1);

            // waite until signal from child 1 is recived
            pause();
        }

        // first child is ready but the second is not reay yet case
        else if (ready == 1)
        {
            // waite until signal from child 2 is recived
            pause();
        }

        // both childs are ready case
        else if (ready == 2)
        {
            // reset counter which is responsible on ready state of the two children
            ready = 0;

            // variable express name of the two text files that include random numbers
            char filenames[] = "child1.txt-child2.txt";

            /*
            * send names of the text files from parent to referee
            */
            if (write(f_des[1], filenames, strlen(filenames)) != -1)
            {
                // wait for the referee to read the pipe and writes the result
                sleep(1);

                // read the results of the round
                if (read(f_des[0], message, BUFSIZ) != -1)
                {

                    // split the returned message and cast it into integers
                    char *token = strtok(message, "-");
                    int score1 = atoi(token);
                    token = strtok(NULL, "-");
                    int score2 = atoi(token);
                    roundNum++;

                    // coloring and printing rounf result
                    reset();
                    printf("_______________________________________________\n");
                    yellow();
                    printf("\n\t      ### Round ( %d ) ###        \n", roundNum);
                    reset();
                    printf("\n\tP1 score = %d   |    P2 score= %d\n\n", score1, score2);
                    fflush(stdout); // To be sure that message will not ignored

                    // increment the BigScores by the round result
                    BigScore1 += score1;
                    BigScore2 += score2;

                    // Show Bigscore at the end of each round
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
    }

    /*
    * Compare BigScore for each child after at least one of them reach 50 to show the final result
    */
    if (BigScore1 >= WIN_SCORE && BigScore2 >= WIN_SCORE)
    {
        green();
        printf("\n================     Draw!     ================\n\n");
    }
    else if (BigScore1 >= WIN_SCORE)
    {
        green();
        printf("\n================   P1 Wins!    ================\n\n");
    }
    else
    {
        green();
        printf("\n================   P2 Wins!    =================\n\n");
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
    // increment ready varuables meaning the number of ready childes has increased by 1
    ready++;
}

/*
 * SIGQUIT signal catcher which it will kill all children then kill the parent
*/
void signal_catcher(int the_sig)
{
    // terminate all children in case any error occured
    for (int i = 0; i < 3; i++)
        kill(SIGKILL, pid_array[i]);

    if (the_sig == SIGQUIT)
        exit(SIGQUIT);
}

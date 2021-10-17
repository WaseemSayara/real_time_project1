
/*
* The Children code
*/

#include "local.h"

void signal_catcher(int);

int main(int argc, char *argv[])
{
    FILE *ptr;
    int i, childNumber;
    long timee;

    // Time multiplied by child number used as seed to the random number which always gives a new values
    // Time is always unique
    childNumber = atoi(argv[0]);
    timee = time(NULL) * childNumber;
    srand(timee);

    // SIGUSR1 catcher
    if (sigset(SIGUSR1, signal_catcher) == SIG_ERR)
    {
        perror("Sigset can not set SIGUSR1");

        // Terminate the program and kill all procceses
        kill(SIGQUIT, getppid());
    }

    /*
    * Create text files for each child 
    * Generate Random numbers and print them on the text files
    */
    while (1)
    {
        // Pause the child to wait for the parent signals to start working
        pause();

        // Concatenate file name using string methods
        char filename[30] = "./child";
        strcat(filename, argv[0]);
        strcat(filename, ".txt");
        ptr = fopen(filename, "w");

        // Print 10 random numbers on the text file
        for (i = 0; i < 10; i++)
        {
            fprintf(ptr, "%d\n", (rand() % 100) + 1);
        }

        // Close file
        fclose(ptr);

        // Sleep a little to put a time space between the Signals from childs
        sleep(childNumber - 1);
        kill(getppid(), SIGINT);
    }
    return 0;
}

void signal_catcher(int the_sig)
{
    /*
    * SIGUSR1 received (child %d start)\n", getpid()
    */
}
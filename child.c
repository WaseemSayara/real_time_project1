
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

    // Time multiplied by child number used as seed to the random number which is always give new values
    // Time always unique
    childNumber = atoi(argv[0]);
    timee = time(NULL) * childNumber;
    srand(timee);

    // SIGUSR catcher
    if (sigset(SIGUSR1, signal_catcher) == SIG_ERR)
    {
        perror("Sigset can not set SIGUSR1");
        exit(SIGUSR1);
    }

    /*
    * Create text files for each child 
    * Generate Random numbers and print them on the text files
    */
    while (1)
    {
        pause();

        // Concatenate file name using string methods
        char filename[30] = "./child";
        strcat(filename, argv[0]);
        strcat(filename, ".txt");
        ptr = fopen(filename, "w+");

        // Print 10 random numbers on the text file
        for (i = 0; i < 10; i++)
        {
            fprintf(ptr, "%d\n", (rand() % 100) + 1);
        }

        fclose(ptr); // Close file to save values

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
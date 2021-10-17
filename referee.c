
/*
* The Referee code
*/

#include "local.h"

int main(int argc, char *argv[])
{

    static char message[BUFSIZ];
    FILE *file1, *file2;

    // Define read(0) & write(1) pipe values
    int pipe_read = atoi(argv[0]);
    int pipe_write = atoi(argv[1]);

    /*
    * Read files from parent and compare every line to specify the score
    */
    while (1)
    {
        // Read message which is contain names of the two files
        if (read(pipe_read, message, BUFSIZ) != -1)
        {

            // Insert file1 name and file2 name in 2 variables
            char file1Name[10], *file2Name;
            char *token = strtok_r(message, "-", &file2Name);
            strcpy(file1Name, message);

            // Open files to read
            file1 = fopen(file1Name, "r");
            file2 = fopen(file2Name, "r");

            // Failed in File process
            if (file1 == NULL || file2 == NULL)
            {
                perror("Can't open a file!");

                // Terminate the program and kill all procceses
                kill(SIGQUIT, getppid());
            }

            // Define temporary and counter vaiables
            int temp1, temp2, counter1 = 0, counter2 = 0;

            // read text files line by line and compare its values
            for (int i = 0; i < 10; i++)
            {
                fscanf(file1, "%d\n", &temp1);
                fscanf(file2, "%d\n", &temp2);
                if (temp1 > temp2)
                    counter1++;
                else if (temp1 < temp2)
                    counter2++;
            }

            // Close files
            fclose(file1);
            fclose(file2);

            // Convert integer to string
            char counter1Str[5], counter2Str[5];
            sprintf(counter1Str, "%d", counter1);
            sprintf(counter2Str, "%d", counter2);

            // Concatenate result in one statment to send it to the parent
            char result[10];
            strcat(result, counter1Str);
            strcat(result, "-");
            strcat(result, counter2Str);

            // Exceptions for deleting files process
            if (remove(message) == -1)
            {
                printf("File 1 is not deleted.\n");

                // Terminate the program and kill all procceses
                kill(SIGQUIT, getppid());
            }

            if (remove(file2Name) == -1)
            {
                printf("File 2 is not deleted.\n");

                // Terminate the program and kill all procceses
                kill(SIGQUIT, getppid());
            }

            // Write result on pipe to the parent
            if (write(pipe_write, result, strlen(result)) != -1)
                sleep(1);

            else
            {
                perror("Write Error!");
                // Terminate the program and kill all procceses
                kill(SIGQUIT, getppid());
            }
        }
        else
        {
            perror("Read Error!");

            // Terminate the program and kill all procceses
            kill(SIGQUIT, getppid());
        }
    }
    return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

int main(int argc, char *argv[])
{

    static char message[BUFSIZ];
    printf("Hello from referee\n");
    fflush(stdout);

    int pipe_read = atoi(argv[0]);
    int pipe_write = atoi(argv[1]);

    FILE *file1;
    FILE *file2;
    printf("0 = %s , 1 = %s \n", argv[0], argv[1]);

    while (1)
    {
        if (read(pipe_read, message, BUFSIZ) != -1)
        {
            char File1Name[10];
            char *File2Name;
            printf("Message received by referee: [%s]\n", message);
            fflush(stdout);
            char *token = strtok_r(message, "-", &File2Name);
            strcpy(File1Name, message);
            // for (int j = 0; j < 10; j++)
            // {
            //     File1Name[j] = message[j];
            // }
            // token = strtok(NULL, " ");
            // strcpy(File2Name, token);

            printf("file1 = %s , file2 = %s \n", File1Name, File2Name);

            file1 = fopen(File1Name, "r");
            file2 = fopen(File2Name, "r");
            if (file1 == NULL || file2 == NULL)
            {
                perror("Can't open a file!");
                return 1;
            }
            int temp1, temp2;
            int counter1 = 0;
            int counter2 = 0;
            for (int i = 0; i < 10; i++)
            {
                fscanf(file1, "%d\n", &temp1);
                fscanf(file2, "%d\n", &temp2);
                if (temp1 > temp2)
                    counter1++;
                else if (temp1 < temp2)
                    counter2++;
            }
            fclose(file1);
            fclose(file2);
            printf("Counter 1 = %d, Counter 2 = %d\n", counter1, counter2);
            char counter1Str[5], counter2Str[5];
            sprintf(counter1Str, "%d", counter1);
            sprintf(counter2Str, "%d", counter2);
            char result[10];
            strcat(result, counter1Str);
            strcat(result, "-");
            strcat(result, counter2Str);

            if (remove(message) == 0)
                printf("The file 1 is deleted successfully.\n");
            else
                printf("The file 1 is not deleted.\n");

            if (remove(File2Name) == 0)
                printf("The file 2 is deleted successfully.\n");
            else
                printf("The file 2 is not deleted.\n");

            sleep(2);
            if (write(pipe_write, result, strlen(result)) != -1)
            {
                printf("Message sent by referee: [%s]\n", result);
                fflush(stdout);
                sleep(1);
            }
            else
            {
                perror("Write");
                exit(2);
            }
        }
        else
        {
            perror("Read");
            exit(1);
        }
    }
    return 0;
}
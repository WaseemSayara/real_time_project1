

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
    printf("0 = %s , 1 = %s \n", argv[0], argv[1]);
    if (read(pipe_read, message, BUFSIZ) != -1)
    {
        printf("Message received by child: [%s]\n", message);
        fflush(stdout);

        char *token = strtok(message, "-");
        FILE *file1 = fopen(token, "r");
        token = strtok(NULL, "-");
        FILE *file2 = fopen(token, "r");
        if (file1 == NULL || file2 == NULL)
        {
            perror("Can't open a file!");
            return;
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
        close(file1);
        close(file2);
        printf("Counter 1 = %d, Counter 2 = %d\n", counter1, counter2);
        char counter1Str[5], counter2Str[5];
        sprintf(counter1Str, "%d", counter1);
        sprintf(counter2Str, "%d", counter2);
        char result[10];
        strcat(result, counter1Str);
        strcat(result, "-");
        strcat(result, counter2Str);
        if (write(pipe_write, result, strlen(result)) != -1)
        {
            printf("Message sent by referee: [%s]\n", result);
            fflush(stdout);
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

    return 0;
}
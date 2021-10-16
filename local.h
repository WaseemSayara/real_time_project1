
/*
*
* Local header which is include all necessary librarys 
* Contains functions to color the outputs 
*
*/

#ifndef __LOCAL_H_
#define __LOCAL_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

/*
* Define colors to style the output
*/

void cyan()
{
    printf("\033[0;36m");
}

void yellow()
{
    printf("\033[1;33m");
}

void reset()
{
    printf("\033[0m");
}

void green()
{
    printf("\033[0;32m");
}
#endif

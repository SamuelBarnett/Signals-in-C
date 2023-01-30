/*======================================================================
|
|
|   Name: signals.c
|
|   Written by: Samuel Barnett - November 2022
|
|   Purpose: To count how many times you pressed ^C and exit when you press ^\
|
|   Compile: gcc -o sig signal.c
|
|   usage:  ./sig
|
|   Description of parameters:
|     no parameters.
|
|
|   Subroutines/libraries required:
|
|      See include statements.
|
|
|------------------------------------------------------------------*/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
// defining global vaiables to keep track of count
#define SIGNAL_MAX 5
int count;
int i;

void COPY_handler(int signo);
void QUIT_handler(int signo);
void SIGUSR1_handler(int signo);

int main()
{
    i = 0;
    count = 0;
    int done = 0;

    // displays message on error if signal() fails
    if (signal(SIGINT, COPY_handler) == SIG_ERR)
    {
        printf("Cannot catch SIGINT.\n");
    }
    if (signal(SIGQUIT, QUIT_handler) == SIG_ERR)
    {
        printf("Cannot catch SIGQUIT.\n");
    }
    if (signal(SIGUSR1, SIGUSR1_handler) == SIG_ERR)
    {
        printf("Cannot catch SIGUSR1.\n");
    }

    while (!done)
    {
        printf("waiting for another signal...\n");
        pause();

        i++;
    }
    // waits for child process
    wait(NULL);
    return 0;
}
/*======================================================================
|
|
|   Function: COPY_handler
|
|   Written by: Samuel Barnett - November 2022
|
|   Purpose: Handles the SIGINT signal, counts how many times you have 
|            pressed ^C and exits after 5 times.
|
|
|   Description of parameters:
|    int signo - the number representing the signal that was called.
|    
|
|   Subroutines/libraries required:
|      none.
|
|
|------------------------------------------------------------------*/
void COPY_handler(int signo)
{
    if (signo == SIGINT)
    {
        // exits if they reached the max amount of signals
        if (i < SIGNAL_MAX)
        {
            count++;
            printf("you have pressed ^C %d times\n", count);
        }
        else
        {
            printf(" \nExiting... you reached the max amount of signals\n");
            printf("you pressed ^C a total of %d times\n", count);
            exit(0);
        }
    }
    else
    {
        // prints error message
        printf("Error recieving signal");
        exit(1);
    }
    return;
};
/*======================================================================
|
|
|   Function: QUIT_handler
|
|   Written by: Samuel Barnett - November 2022
|
|   Purpose: Handles the SIGQUIT signal
|            
|
|
|   Description of parameters:
|    int signo - the number representing the signal that was called.
|    
|
|   Subroutines/libraries required:
|      none.
|
|
|------------------------------------------------------------------*/
void QUIT_handler(int signo)
{
    if (signo == SIGQUIT)
    {
        printf(" \nyou have pressed ^/.\n");
        int firstChild = fork();
        // inside the parent process
        if (firstChild != 0)
        {
            return;
        }
        // Inside the first child process.
        else if (firstChild == 0)
        {
            // calling kill() on parent and sending SIGUSR1
            if (kill(getppid(), SIGUSR1) == -1)
            {
                printf("Cannot run kill().\n");
            }
            else
            {
                printf("This is the child process and I am sending a signal\n");
            }
        }
    }
    else
    {
        // displays error message
        printf("Error recieving signal");
        exit(1);
    }
    return;
};
/*======================================================================
|
|
|   Function: SIGUSR1_handler
|
|   Written by: Samuel Barnett - November 2022
|
|   Purpose: Handles the SIGUSR1 signal.
|
|
|   Description of parameters:
|    int signo - the number representing the signal that was called.
|    
|
|   Subroutines/libraries required:
|      none.
|
|
|------------------------------------------------------------------*/
void SIGUSR1_handler(int signo)
{
    // handles SIGUSR1 by simply displaying a message and exiting the program
    if (signo == SIGUSR1)
    {
        printf("\nThe program is now over :(\n");
        printf("you pressed ^C a total of %d times\n", count);
        exit(0);
    }
};

#include<stdlib.h>  
#include<stdio.h>
#include<limits.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include <sys/time.h>

#define CHILDS_AMOUNT 2
int messages_amount = 0;

char *get_time()
{
    struct timeval curTime;
    if (gettimeofday(&curTime, NULL) == -1)
    {
        perror("Get time error: \n");
        return NULL;
    }
    int milli = curTime.tv_usec / 1000;

    char buffer [9];
    if (strftime(buffer, 9, "%H:%M:%S", localtime(&curTime.tv_sec)) == 0)
    {
        fprintf(stderr, "Array exciding error");
        return NULL;
    }

    char *currentTime = malloc(sizeof(char)*13);
    if (sprintf(currentTime, "%s:%03d", buffer, milli) < 0)
    {
        fprintf(stderr, "Array exciding error");
        return NULL;
    }
    return currentTime;
}

void sig_handler_parent(int signum, siginfo_t *info, void *ucontext){
    char *time = get_time();
    printf("[%s] PARENT: [%d] [PID: %d] [PPID: %d] [CHILD: %d]\n", time, messages_amount++, getpid(), getppid(), info->si_pid); 
    free(time);
    usleep(100 * 1000);
    kill(0, SIGUSR1);
}

void sig_handler_child(int signum, siginfo_t *info, void *ucontext){
    char *time = get_time();
    printf("[%s] CHILD: [%d] [PID: %d] [PPID: %d]\n", time, messages_amount++, getpid(), getppid());
    free(time);
    kill(getppid(),SIGUSR2);
}


int main()
{
    pid_t child_pid;    
    struct sigaction sigact, sigact_child;
    sigact.sa_flags = SA_SIGINFO;
    sigact.sa_sigaction = sig_handler_parent;
    if (sigaction(SIGUSR2, &sigact, NULL) == -1)
    {
        perror("Failed to set handler:");
    };
    if (signal(SIGUSR1, SIG_IGN) == SIG_ERR)
    {
        perror("Failed to set mask:");
    };

    for (int i = 0; i < CHILDS_AMOUNT; i++)
    {
        child_pid = fork();

        switch (child_pid)
        {
            case 0:   
                sigact_child.sa_flags = SA_SIGINFO;
                sigact_child.sa_sigaction = sig_handler_child;
                if (sigaction(SIGUSR1, &sigact_child, NULL) == -1)
                {
                    perror("Failed to set handler:");
                };
                while(1)
                sleep(2);
                break;

            case -1:
                perror("Fork error:\n");
                exit(-1);
                default:
                break;
        }
    }

    kill(0, SIGUSR1);

    while(1)
    sleep(2);
}
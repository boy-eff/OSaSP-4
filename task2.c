#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<sys/time.h>
#include<sys/wait.h>

#define TOTAL_SIGNALS 100

//process groups are 1, 2, 4, 5, 8
pid_t processes[10];
int signal_num = 0;

struct sigaction sigact1, sigact2, sigact3, sigact4, sigact5, sigact6, sigact7, sigact8;

struct sigaction sigterm_action2, sigterm_action3, sigterm_action4, sigterm_action5, sigterm_action6, sigterm_action7, sigterm_action8;

char *curr_time;



void sigterm_handler2(int signum, siginfo_t *info, void *ucontext)
{
    printf("[PID: %d] [PPID: %d] Process was killed, signals amount - %d\n", getpid(), getppid(), signal_num);
    if (kill(getpid(), SIGKILL) == -1)
    {
        perror("");
    };
}

void sigterm_handler3(int signum, siginfo_t *info, void *ucontext)
{
    printf("[PID: %d] [PPID: %d] Process was killed, signals amount - %d\n", getpid(), getppid(), signal_num);
    if (kill(processes[4], SIGTERM) == -1)
    {
        perror("");
    };
    if (waitpid(processes[4], NULL, 0) == -1)
    {
        perror("");
    };
    if (kill(getpid(), SIGKILL) == -1)
    {
        perror("");
    };
}

void sigterm_handler4(int signum, siginfo_t *info, void *ucontext)
{
    printf("[PID: %d] [PPID: %d] Process was killed, signals amount - %d\n", getpid(), getppid(), signal_num);
    if (kill(-processes[5], SIGTERM) == -1)
    {
        perror("");
    };
    if (waitpid(processes[7], NULL, 0) == -1)
    {
        perror("");
    };
    if (kill(getpid(), SIGKILL) == -1)
    {
        perror("");
    };
}

void sigterm_handler5(int signum, siginfo_t *info, void *ucontext)
{
    printf("[PID: %d] [PPID: %d] Process was killed, signals amount - %d\n", getpid(), getppid(), signal_num);
    if (kill(getpid(), SIGKILL) == -1)
    {
        perror("");
    };
}

void sigterm_handler6(int signum, siginfo_t *info, void *ucontext)
{
    printf("[PID: %d] [PPID: %d] Process was killed, signals amount - %d\n", getpid(), getppid(), signal_num);
    if (kill(getpid(), SIGKILL) == -1)
    {
        perror("");
    };
}

void sigterm_handler7(int signum, siginfo_t *info, void *ucontext)
{
    printf("[PID: %d] [PPID: %d] Process was killed, signals amount - %d\n", getpid(), getppid(), signal_num);
    if (kill(processes[8], SIGTERM) == -1)
    {
        perror("");
    };
    if (waitpid(processes[8], NULL, 0) == -1)
    {
        perror("");
    };
    if (kill(getpid(), SIGKILL) == -1)
    {
        perror("");
    };
}

void sigterm_handler8(int signum, siginfo_t *info, void *ucontext)
{
    printf("[PID: %d] [PPID: %d] Process was killed, signals amount - %d\n", getpid(), getppid(), signal_num);
    if (kill(getpid(), SIGKILL) == -1)
    {
        perror("");
    };
}

char *get_time()
{
    struct timeval curTime;
    if (gettimeofday(&curTime, NULL) == -1)
    {
        perror("Get time error: \n");
        return NULL;
    }
    int micro = curTime.tv_usec;

    char buffer [9];
    if (strftime(buffer, 9, "%H:%M:%S", localtime(&curTime.tv_sec)) == 0)
    {
        fprintf(stderr, "Array exciding error");
        return NULL;
    }

    char *currentTime = malloc(sizeof(char)*17);
    if (sprintf(currentTime, "%s:%06d", buffer, micro) < 0)
    {
        fprintf(stderr, "Array exciding error");
        return NULL;
    }
    return currentTime;
}

void node1_handler(int signum, siginfo_t *info, void *ucontext)
{
    curr_time = get_time();
    printf("[%s][NODE 1] [PID: %d] [PPID: %d] GET SIGNAL USR2\n", curr_time, getpid(), getppid());
    free(curr_time);
    fflush(stdout);
    if (signal_num++ == TOTAL_SIGNALS)
    {
        if (kill(-processes[2], SIGTERM) == -1)
        {
            perror("");
        };
        int wstatus;
        if (waitpid(processes[2], &wstatus, 0) == -1)
        {
            perror("");
        };
        printf("[PID: %d] [PPID: %d] Process was killed, signals amount - %d\n", getpid(), getppid(), signal_num);
        if (kill(getpid(), SIGKILL) == -1)
        {
            perror("");
        };
    }
    else
    {
        printf("[%s][NODE 1] [PID: %d] [PPID: %d] SEND SIGNAL USR1\n", curr_time, getpid(), getppid());
        if (kill(-processes[2], SIGUSR1) == -1)
        {
            perror("");
        };
    }
}

void node2_handler(int signum, siginfo_t *info, void *ucontext)
{
    signal_num++;
    curr_time = get_time();
    printf("[%s][NODE 2] [PID: %d] [PPID: %d] GET SIGNAL USR1\n", curr_time, getpid(), getppid());
    free(curr_time);
    fflush(stdout);
}

void node3_handler(int signum, siginfo_t *info, void *ucontext)
{
    signal_num++;
    curr_time = get_time();
    printf("[%s][NODE 3] [PID: %d] [PPID: %d] GET SIGNAL USR1\n", curr_time, getpid(), getppid());
    free(curr_time);
    fflush(stdout);
    printf("[%s][NODE 3] [PID: %d] [PPID: %d] SEND SIGNAL USR2\n", curr_time, getpid(), getppid());
    if (kill(processes[4], SIGUSR2) == -1)
    {
        perror("");
    };
}

void node4_handler(int signum, siginfo_t *info, void *ucontext)
{
    signal_num++;
    curr_time = get_time();
    printf("[%s][NODE 4] [PID: %d] [PPID: %d] GET SIGNAL USR2\n", curr_time, getpid(), getppid());
    free(curr_time);
    fflush(stdout);
    printf("[%s][NODE 4] [PID: %d] [PPID: %d] SEND SIGNAL USR1\n", curr_time, getpid(), getppid());
    if (kill(-processes[5], SIGUSR1) == -1)
    {
        perror("");
    };
}

void node5_handler(int signum, siginfo_t *info, void *ucontext)
{
    signal_num++;
    curr_time = get_time();
    printf("[%s][NODE 5] [PID: %d] [PPID: %d] GET SIGNAL USR1\n", curr_time, getpid(), getppid());
    free(curr_time);
    fflush(stdout);
}

void node6_handler(int signum, siginfo_t *info, void *ucontext)
{
    signal_num++;
    curr_time = get_time();
    printf("[%s][NODE 6] [PID: %d] [PPID: %d] GET SIGNAL USR1\n", curr_time, getpid(), getppid());
    free(curr_time);
    fflush(stdout);
}

void node7_handler(int signum, siginfo_t *info, void *ucontext)
{
    signal_num++;
    curr_time = get_time();
    printf("[%s][NODE 7] [PID: %d] [PPID: %d] GET SIGNAL USR1\n", curr_time, getpid(), getppid());
    free(curr_time);
    fflush(stdout);
    printf("[%s][NODE 7] [PID: %d] [PPID: %d] SEND SIGNAL USR1\n", curr_time, getpid(), getppid());
    if (kill(processes[8], SIGUSR1) == -1)
    {
        perror("");
    };
}

void node8_handler(int signum, siginfo_t *info, void *ucontext)
{
    signal_num++;
    curr_time = get_time();
    printf("[%s][NODE 8] [PID: %d] [PPID: %d] GET SIGNAL USR1\n", curr_time, getpid(), getppid());
    free(curr_time);
    fflush(stdout);
    printf("[%s][NODE 8] [PID: %d] [PPID: %d] SEND SIGNAL USR2\n", curr_time, getpid(), getppid());
    if (kill(processes[1], SIGUSR2) == -1)
    {
        perror("");
    };
}

void init_sigacts()
{
    sigact1.sa_flags = SA_SIGINFO;
    sigact1.sa_sigaction = node1_handler;

    sigact2.sa_flags = SA_SIGINFO;
    sigact2.sa_sigaction = node2_handler;

    sigact3.sa_flags = SA_SIGINFO;
    sigact3.sa_sigaction = node3_handler;

    sigact4.sa_flags = SA_SIGINFO;
    sigact4.sa_sigaction = node4_handler;

    sigact5.sa_flags = SA_SIGINFO;
    sigact5.sa_sigaction = node5_handler;

    sigact6.sa_flags = SA_SIGINFO;
    sigact6.sa_sigaction = node6_handler;

    sigact7.sa_flags = SA_SIGINFO;
    sigact7.sa_sigaction = node7_handler;

    sigact8.sa_flags = SA_SIGINFO;
    sigact8.sa_sigaction = node8_handler;

    sigterm_action2.sa_flags = SA_SIGINFO;
    sigterm_action2.sa_sigaction = sigterm_handler2;
    
    sigterm_action3.sa_flags = SA_SIGINFO;
    sigterm_action3.sa_sigaction = sigterm_handler3;
    
    sigterm_action4.sa_flags = SA_SIGINFO;
    sigterm_action4.sa_sigaction = sigterm_handler4;

    sigterm_action5.sa_flags = SA_SIGINFO;
    sigterm_action5.sa_sigaction = sigterm_handler5;
    
    sigterm_action6.sa_flags = SA_SIGINFO;
    sigterm_action6.sa_sigaction = sigterm_handler6;

    sigterm_action7.sa_flags = SA_SIGINFO;
    sigterm_action7.sa_sigaction = sigterm_handler7;

    sigterm_action8.sa_flags = SA_SIGINFO;
    sigterm_action8.sa_sigaction = sigterm_handler8;
}

void create_tree()
{
    pid_t child_pid;
    //1
    child_pid = fork();
    switch(child_pid)
    {
        case 0:
        //2,3
        processes[1] = getpid();
        
        if (sigaction(SIGUSR2, &sigact1, NULL) == -1)
        {
            perror("Failed to set handler:");
        };
        for  (int i = 0; i < 2; i++)
        {
            child_pid = fork();
            switch(child_pid)
            {
                case 0:
                processes[2 + i] = getpid();
                if (i == 0)
                {
                    if (setpgid(0, 0) == -1)
                    {
                        perror("");
                    };
                    
                    if (sigaction(SIGUSR1, &sigact2, NULL) == -1  || (sigaction(SIGTERM, &sigterm_action2, NULL) == -1))
                    {
                        perror("Failed to set handler:");
                    };
                }
                else
                {
                    sleep(1);
                    if (setpgid(0, processes[2]) == -1)
                    {
                        perror("");
                    };
                    
                    if (sigaction(SIGUSR1, &sigact3, NULL) == -1  || (sigaction(SIGTERM, &sigterm_action3, NULL) == -1))
                    {
                        perror("Failed to set handler:");
                    };
                    //4
                    child_pid = fork();
                    switch(child_pid)
                    {
                        case 0:
                        //5,6,7
                        if (setpgid(0, 0) == -1)
                        {
                            perror("");
                        };
                        
                        if (sigaction(SIGUSR2, &sigact4, NULL) == -1  || (sigaction(SIGTERM, &sigterm_action4, NULL) == -1))
                        {
                            perror("Failed to set handler:");
                        };
                        processes[4] = getpid();
                        for (int i = 0; i < 3; i++)
                        {
                            child_pid = fork();
                            switch(child_pid)
                            {
                                case 0:
                                if (i == 0)
                                {
                                    if (setpgid(0, 0) == -1)
                                    {
                                        perror("");
                                    };
                                    
                                    if (sigaction(SIGUSR1, &sigact5, NULL) == -1 || (sigaction(SIGTERM, &sigterm_action5, NULL) == -1)) 
                                    {
                                        perror("Failed to set handler:");
                                    };
                                }
                                else
                                {
                                    if (i == 1)
                                    {
                                        
                                        if (sigaction(SIGUSR1, &sigact6, NULL) == -1 || (sigaction(SIGTERM, &sigterm_action6, NULL) == -1))
                                        {
                                            perror("Failed to set handler:");
                                        };
                                    }
                                    if (i == 2)
                                    {
                                        
                                        if (sigaction(SIGUSR1, &sigact7, NULL) == -1 || (sigaction(SIGTERM, &sigterm_action7, NULL) == -1))
                                        {
                                            perror("Failed to set handler:");
                                        };
                                    }
                                    sleep(1);
                                    if (setpgid(0, processes[5]) == -1)
                                    {
                                        perror("");
                                    };
                                }
                                //8
                                processes[5 + i] = getpid();
                                if (i == 2)
                                {
                                    child_pid = fork();
                                    switch(child_pid)
                                    {
                                        case 0:
                                        if (setpgid(0, 0) == -1)
                                        {
                                            perror("");
                                        };
                                        
                                        if (sigaction(SIGUSR1, &sigact8, NULL) == -1 || (sigaction(SIGTERM, &sigterm_action8, NULL) == -1))
                                        {
                                            perror("Failed to set handler:");
                                        };
                                        processes[8] = getpid();
                                        while(1)
                                        sleep(2);
                                        break;
                                        case -1:
                                        perror("Fork error:\n");
                                        exit(-1);
                                        default:
                                        printf("PROCESS %d SUCCESSFULLY CREATED\n", child_pid);
                                        processes[8] = child_pid;
                                        kill(processes[1], SIGUSR2);
                                        break;
                                    }
                                }
                                while(1)
                                sleep(2);
                                break;
                                case -1:
                                perror("Fork error:\n");
                                exit(-1);
                                default:
                                printf("PROCESS %d SUCCESSFULLY CREATED\n", child_pid);
                                processes[5 + i] = child_pid;
                                break;
                            }
                        }
                        while(1)
                        sleep(2);
                        break;
                        case -1:
                        perror("Fork error:\n");
                        exit(-1);
                        default:
                        printf("PROCESS %d SUCCESSFULLY CREATED\n", child_pid);
                        processes[4] = child_pid;
                        break;
                    }
                }
                while(1)
                sleep(2);
                break;
                case -1:
                    perror("Fork error:\n");
                    exit(-1);
                default:
                    printf("PROCESS %d SUCCESSFULLY CREATED\n", child_pid);
                    processes[2 + i] = child_pid;
                    break;
            }
        }
        
        while(1)
        sleep(2);
        break;
        case -1:
        perror("Fork error:\n");
        exit(-1);
        default:
        printf("PROCESS %d SUCCESSFULLY CREATED\n", child_pid);
        processes[1] = child_pid;
        waitpid(processes[1], NULL, 0);
        printf("Main program has killed");
        break;
    }
}

int main()
{
    init_sigacts();
    create_tree();
}
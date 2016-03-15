#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>

char log[100][100];
int logIndex = 0;

void sigusronehandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGUSR1 from %d\n", info->si_pid );
}

void sigusrtwohandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGUSR2 from %d\n", info->si_pid );
}

void sighuphandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGHUP from %d\n", info->si_pid );
}

void sigchldhandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGCHLD from %d with following fields:\n", info->si_pid);
    printf("Signal value: %d\n", info->si_value);
    printf("Error number: %d\n", info->si_errno);
    printf("UID of sender: %d\n", info->si_uid);
    printf("Address of fault: %d\n", info->si_addr);
    printf("Status: %d\n", info->si_status);
    printf("Band event: %d\n", info->si_band);
}

void sigchldhandlerPOSIX(int signum, siginfo_t* info, void* f) {
    printf("Parent prints:\n");
    for (int i = 0;i<logIndex;i++) {
        printf("%s\n", log[i]);
    } 
}

void sigusronehandlerPOSIX(int signum, siginfo_t* info, void* f) {
    sprintf(log[logIndex], "%d | %d | %d | %d ", logIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    logIndex++;
}

void sigusrtwohandlerPOSIX(int signum, siginfo_t* info, void* f) {
    sprintf(log[logIndex], "%d | %d | %d | %d ", logIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    logIndex++;
}

void sighuphandlerPOSIX(int signum, siginfo_t* info, void* f) {
    sprintf(log[logIndex], "%d | %d | %d | %d ", logIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    logIndex++;
}



int main(int argc, char** argv) {
    
    int c, argSize = 0;
    char* mode;
    char* signalToSend;
    char* processToKill;
    char* signalsAmount;
    
    time_t t;
    srand((unsigned) time(&t));
    
    while (1) {
        int option_index = 0;
        
        static struct option long_options[] = {
             {"mode", required_argument, 0,  0 },
             {"signal", required_argument, 0, 0},
             {"pid", required_argument, 0, 0},
             {"amount", required_argument, 0, 0}
        };
        
        
        c = getopt_long(argc, argv, "m",
                        long_options, &option_index);
        
        if (c == -1)
            break;
        else {
            if (strcmp(long_options[option_index].name, "mode") == 0 ) {
                mode = optarg;
            } else if (strcmp(long_options[option_index].name, "signal") == 0) {
                signalToSend = optarg;
            } else if (strcmp(long_options[option_index].name, "pid") == 0) {
                processToKill = optarg;
            } else if (strcmp(long_options[option_index].name, "amount") == 0 ) {
                signalsAmount = optarg;
            }
        }
    }
    
    if (strcmp(mode, "pipe") == 0) {
        printf("Pipe mode activated\n");
    } else if (strcmp(mode, "child") == 0) {
        struct sigaction actSIGCHLD;
        
        memset(&actSIGCHLD, 0, sizeof(actSIGCHLD));
        actSIGCHLD.sa_sigaction = sigchldhandler;
        actSIGCHLD.sa_flags = SA_SIGINFO;
        sigaction(SIGCHLD, &actSIGCHLD, NULL);
        
        pid_t pid = fork();
        if (0 == pid) {
            int sleepTime = rand() % 10;
            printf("Child is sleeping for a %d seconds\n",sleepTime);
            sleep(sleepTime);
            exit(13);
        } else if (pid > 0) {
            while(1){
            }
        }
    } else if (strcmp(mode, "std") == 0) {
        
        struct sigaction actSIGUSRONE;
        struct sigaction actSIGUSRTWO;
        struct sigaction actSIGHUP;
    
        memset(&actSIGUSRONE, 0, sizeof(actSIGUSRONE));
        actSIGUSRONE.sa_sigaction = sigusronehandler;
        actSIGUSRONE.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR1, &actSIGUSRONE, NULL);
        
        memset(&actSIGUSRTWO, 0, sizeof(actSIGUSRTWO));
        actSIGUSRTWO.sa_sigaction = sigusrtwohandler;
        actSIGUSRTWO.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR2, &actSIGUSRTWO, NULL);
        
        memset(&actSIGHUP, 0, sizeof(actSIGHUP));
        actSIGHUP.sa_sigaction = sighuphandler;
        actSIGHUP.sa_flags = SA_SIGINFO;
        sigaction(SIGHUP, &actSIGHUP, NULL);
        
        while (1) {
        } 
        
    } else if (strcmp(mode, "kill") == 0) {        
        kill(atoi(processToKill), atoi(signalToSend));
    } else if (strcmp(mode, "posix") == 0) {
        pid_t chld_pid = fork();
        if (0 == chld_pid) {
            int amountInt = atoi(signalsAmount);
            union sigval val;
            
            printf("Child prints:\n");
            for(int i=0;i<amountInt;i++) {
                int diceRoll = rand()%3;
                int nextSignal;
                switch(diceRoll) {
                    case 0:
                        nextSignal = SIGUSR1;
                        break;
                    case 1:
                        nextSignal = SIGUSR2;
                        break;
                    case 2:
                        nextSignal = SIGHUP;
                        break;
                }
                
                val.sival_int = rand()%100;
                sigqueue(getppid(),nextSignal,val);
                printf("%d | %d | %d | %d | %d\n", i+1, getpid(), getppid(), nextSignal, val.sival_int);
                sleep(5);
            }
            exit(13);
        } else if (chld_pid > 0) {
            struct sigaction actSIGCHLD;
        
            memset(&actSIGCHLD, 0, sizeof(actSIGCHLD));
            actSIGCHLD.sa_sigaction = sigchldhandlerPOSIX;
            actSIGCHLD.sa_flags = SA_SIGINFO;
            sigaction(SIGCHLD, &actSIGCHLD, NULL);
            
            struct sigaction actSIGUSRONE;
            struct sigaction actSIGUSRTWO;
            struct sigaction actSIGHUP;
    
            memset(&actSIGUSRONE, 0, sizeof(actSIGUSRONE));
            actSIGUSRONE.sa_sigaction = sigusronehandlerPOSIX;
            actSIGUSRONE.sa_flags = SA_SIGINFO;
            sigaction(SIGUSR1, &actSIGUSRONE, NULL);
        
            memset(&actSIGUSRTWO, 0, sizeof(actSIGUSRTWO));
            actSIGUSRTWO.sa_sigaction = sigusrtwohandlerPOSIX;
            actSIGUSRTWO.sa_flags = SA_SIGINFO;
            sigaction(SIGUSR2, &actSIGUSRTWO, NULL);
        
            memset(&actSIGHUP, 0, sizeof(actSIGHUP));
            actSIGHUP.sa_sigaction = sighuphandlerPOSIX;
            actSIGHUP.sa_flags = SA_SIGINFO;
            sigaction(SIGHUP, &actSIGHUP, NULL);
            
            while(1) {
            }
        }
    }
    
    
    return 0;
}

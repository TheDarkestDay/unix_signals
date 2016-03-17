#include <stdio.h>  
#include <stdlib.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h> 
#include <time.h>
#include <common.h>

char signalsLog[100][100];
int logIndex = 0;


void sigchldhandlerPOSIX(int signum, siginfo_t* info, void* f) {
    printf("Parent prints:\n");
    for (int i = 0;i<logIndex;i++) {
        printf("%s\n", signalsLog[i]);
    } 
}

void sigusronehandlerPOSIX(int signum, siginfo_t* info, void* f) {
    sprintf(signalsLog[logIndex], "%d | %d | %d | %d ", logIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    logIndex++;
}

void sigusrtwohandlerPOSIX(int signum, siginfo_t* info, void* f) {
    sprintf(signalsLog[logIndex], "%d | %d | %d | %d ", logIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    logIndex++;
}

void sighuphandlerPOSIX(int signum, siginfo_t* info, void* f) {
    sprintf(signalsLog[logIndex], "%d | %d | %d | %d ", logIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    logIndex++;
}



void performPOSIX(char* signalsAmount) {
	time_t t;
	srand((unsigned)time(&t));

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
             //   sleep(5);
            }
            exit(13);
        } else if (chld_pid > 0) {
            struct sigaction actSIGCHLD;
        
            memset(&actSIGCHLD, 0, sizeof(actSIGCHLD));
            actSIGCHLD.sa_sigaction = sigchldhandlerPOSIX;
            actSIGCHLD.sa_flags = SA_SIGINFO;
            if (sigaction(SIGCHLD, &actSIGCHLD, NULL) == -1) {
                perror("Signal handler assignment failed");
                exit(1);
            };
		
            
            struct sigaction actSIGUSRONE;
            struct sigaction actSIGUSRTWO;
            struct sigaction actSIGHUP;
    
            memset(&actSIGUSRONE, 0, sizeof(actSIGUSRONE));
            actSIGUSRONE.sa_sigaction = sigusronehandlerPOSIX;
            actSIGUSRONE.sa_flags = SA_SIGINFO;
            if (sigaction(SIGUSR1, &actSIGUSRONE, NULL) == -1) {
                perror("Signal handler assignment failed");
                exit(1);
            };
        
            memset(&actSIGUSRTWO, 0, sizeof(actSIGUSRTWO));
            actSIGUSRTWO.sa_sigaction = sigusrtwohandlerPOSIX;
            actSIGUSRTWO.sa_flags = SA_SIGINFO;
            if (sigaction(SIGUSR2, &actSIGUSRTWO, NULL) == -1) {
                perror("Signal handler assignment failed");
                exit(1);
            };
        
            memset(&actSIGHUP, 0, sizeof(actSIGHUP));
            actSIGHUP.sa_sigaction = sighuphandlerPOSIX;
            actSIGHUP.sa_flags = SA_SIGINFO;
            if (sigaction(SIGHUP, &actSIGHUP, NULL) == -1) {
                perror("Signal handler assignment failed");
                exit(1);
            }
            
	    sigfillset(&actSIGHUP.sa_mask);

            while(1) {
            }
}
}

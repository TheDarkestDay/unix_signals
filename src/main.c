#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>

void sigusronehandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGUSR1 from %d\n", info->si_pid );
}

void sigusrtwohandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGUSR2 from %d\n", info->si_pid );
}

void sighuphandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGHUP from %d\n", info->si_pid );
}



int main(int argc, char** argv) {
    
    int c, argSize = 0;
    char* mode;
    char* signalToSend;
    char* processToKill;
    char* signalsAmount;
    
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
        printf("Child mode activated\n");
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
            sleep(1);
          /*  raise(SIGUSR2);
            raise(SIGUSR1);
            raise(SIGHUP); */
        } 
        
    } else if (strcmp(mode, "kill") == 0) {
        printf("%s\n", processToKill);
        printf("%s\n", signalToSend);
        
        kill(atoi(processToKill), atoi(signalToSend));
    } else if (strcmp(mode, "posix") == 0) {
        printf("POSIX mode activated");
    }
    
    
    return 0;
}

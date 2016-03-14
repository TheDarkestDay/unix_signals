#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>

void sigusronehandler(int signum, siginfo_t* info, void* f) {
    printf("We tried access %p, ", info->si_addr);
}



int main(int argc, char** argv) {
    
    int c;
    char* mode;
    
    while (1) {
        int option_index = 0;
        
        static struct option long_options[] = {
             {"mode", required_argument, 0,  0 },
        };
        
        
        c = getopt_long(argc, argv, "m",
                        long_options, &option_index);
        
        if (c == -1)
            break;
        else {
            mode = optarg;
        }
    }
    
    if (strcmp(mode, "pipe") == 0) {
        printf("Pipe mode activated\n");
    } else if (strcmp(mode, "child") == 0) {
        printf("Child mode activated\n");
    } else if (strcmp(mode, "std") == 0) {
        printf("Std mode activated\n");
    } else if (strcmp(mode, "kill") == 0) {
        printf("Kill mode activated\n");
    } else if (strcmp(mode, "posix") == 0) {
        printf("POSIX mode activated");
    }
    
 /*   struct sigaction act;
    
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = sigusronehandler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    
   // raise(SIGUSR1); */
    
    
    return 0;
}

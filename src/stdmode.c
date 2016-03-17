#include <stdio.h>  
#include <stdlib.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <string.h> 
#include <errno.h>

void sigusronehandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGUSR1 from %d\n", info->si_pid );
}

void sigusrtwohandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGUSR2 from %d\n", info->si_pid );
}

void sighuphandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGHUP from %d\n", info->si_pid );
}

void stdPerform() {
   	struct sigaction actSIGUSRONE;
        struct sigaction actSIGUSRTWO;
        struct sigaction actSIGHUP;
    
        memset(&actSIGUSRONE, 0, sizeof(actSIGUSRONE));
        actSIGUSRONE.sa_sigaction = sigusronehandler;
        actSIGUSRONE.sa_flags = SA_SIGINFO;
        if (sigaction(SIGUSR1, &actSIGUSRONE, NULL) == -1) {
            perror("Signal handler assignment failed");
            exit(1);
        };
        
        memset(&actSIGUSRTWO, 0, sizeof(actSIGUSRTWO));
        actSIGUSRTWO.sa_sigaction = sigusrtwohandler;
        actSIGUSRTWO.sa_flags = SA_SIGINFO;
        if (sigaction(SIGUSR2, &actSIGUSRTWO, NULL) == -1) {
            perror("Signal handler assignment failed");
            exit(1);
        };
        
        memset(&actSIGHUP, 0, sizeof(actSIGHUP));
        actSIGHUP.sa_sigaction = sighuphandler;
        actSIGHUP.sa_flags = SA_SIGINFO;
        if (sigaction(SIGHUP, &actSIGHUP, NULL)) {
            perror("Signal handler assignment failed");
            exit(1);
        };
        
        while (1) {
	    raise(SIGUSR1);
	    raise(SIGUSR2);
            raise(SIGHUP);
            sleep(3); 	
        } 
}

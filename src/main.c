 #include "stdio.h" 
 #include "stdlib.h" 
 #include "string.h" 
 #include "sys/signal.h" 

void sigusronehandler(int signum, siginfo_t* info, void* f) {
    printf("We tried access %p, ", info->si_addr);
}



int main(int argc, char* argv[]) {
    struct sigaction act;
    
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = sigusronehandler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    
   // raise(SIGUSR1);
    
    
    return 0;
}

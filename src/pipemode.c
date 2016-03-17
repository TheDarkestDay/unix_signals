#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>

void sigpipehandler(int signum, siginfo_t* info, void* f) {
    printf("Pipe closed\n");
}

void performPipe() {
	struct sigaction actSIGPIPE;
        
        memset(&actSIGPIPE, 0, sizeof(actSIGPIPE));
        actSIGPIPE.sa_sigaction = sigpipehandler;
        actSIGPIPE.sa_flags = SA_SIGINFO;
        if (sigaction(SIGPIPE, &actSIGPIPE, NULL) == -1) {
            perror("Signal handler assignment failed");
            exit(1);
        };
        
        int fd[2];
        pid_t reader_pid;
        char msg[] = "Testing pipe";
        
        if (pipe(fd) < 0) {
            perror("Pipe creation failed");
            exit(1);
        };
        reader_pid = fork();
        
        if(reader_pid == -1)
        {
            perror("Forking child failed");
            exit(1);
        }
        
        if (reader_pid == 0) {
            close(fd[1]);
            close(fd[0]);
            exit(0);
        } else if (reader_pid > 0) {
            close(fd[0]);
            sleep(5);
            if (write(fd[1], msg, (strlen(msg)+1)) == -1) {
                perror("Writing failed");
                exit(1);
            };
        }
        
        while(1) {
            
        }
}

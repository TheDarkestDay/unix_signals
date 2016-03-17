#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>  
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>


void sigchldhandler(int signum, siginfo_t* info, void* f) {
    printf("Received SIGCHLD from %d with following fields:\n", info->si_pid);
    printf("Signal value: %d\n", info->si_value.sival_int);
    printf("Error number: %d\n", info->si_errno);
    printf("UID of sender: %d\n", info->si_uid);
    printf("Status: %d\n", info->si_status);
}

void performChild() {
	struct sigaction actSIGCHLD;
        
        memset(&actSIGCHLD, 0, sizeof(actSIGCHLD));
        actSIGCHLD.sa_sigaction = sigchldhandler;
        actSIGCHLD.sa_flags = SA_SIGINFO;
        if (sigaction(SIGCHLD, &actSIGCHLD, NULL) == -1) {
            perror("Signal handler assignment failed");
            exit(1);
        };
        
        pid_t pid = fork();
        if (0 == pid) {
            int sleepTime = rand() % 10;
            printf("Child is sleeping for a %d seconds\n",sleepTime);
            sleep(sleepTime);
            exit(13);
        } else if (pid > 0) {
	    int status;
            if (0<wait(&status)) {
		int exit_way = WIFEXITED(status);
		int exit_code = WEXITSTATUS(status);
		fprintf(stderr, "Exit Way: %d, Exit Code: %d\n", exit_way, exit_code);
		exit(0);
	    } else {
		perror("Failed handling child");
		exit(1);
	    }
        } else {
            perror("Forking child failed");
            exit(1);
        }
}

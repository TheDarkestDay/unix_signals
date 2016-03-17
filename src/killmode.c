#include <stdio.h>  
#include <stdlib.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>

void performKill(char* processToKill, char* signalToSend) {
	if (kill(atoi(processToKill), atoi(signalToSend)) == -1) {
            perror("Signal sending failed");
            exit(1);
        };
}

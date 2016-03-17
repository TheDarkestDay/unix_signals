#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <getopt.h>
#include <common.h>


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
        performPipe();
    } else if (strcmp(mode, "child") == 0) {
        performChild();
    } else if (strcmp(mode, "std") == 0) {
        stdPerform();       
    } else if (strcmp(mode, "kill") == 0) {        
        performKill(processToKill,signalToSend);
    } else if (strcmp(mode, "posix") == 0) {
        performPOSIX(signalsAmount);
    }
    
    
    return 0;
}

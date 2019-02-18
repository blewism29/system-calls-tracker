// System includes
#include <stdio.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h>

// Custom includes
#include "syscall.h"

// Main
int main()
{   
    // Properties
    long orig_rax;
    int status;
    int isNotInCall = 1;

    pid_t child = fork();

    // Main Process
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("./program", "prog", NULL); // Replace current process with new program.
    }
    else {
        while(1) {
            wait(&status);
            if(WIFEXITED(status)) break;
          
            orig_rax = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
            if (isNotInCall) {
                isNotInCall = 0;
                printf("System call made: %s\n", callname(orig_rax));
            } else {
                isNotInCall = 1;
            }
          
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
       }
    }
    return 0;
}
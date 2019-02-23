// System includes
#include <stdio.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h>

// Custom includes
#include "utilities/syscall.h"
#include "utilities/table.h"

// Main
int main()
{   
    // Properties
    long orig_rax;
    
    // Flags
    int status;
    int isNotInCall = 1;
    int vOptionActive = 1; // flag for option v (V causes this to be active as well)
    int VOptionActive = 1; // flag for option V

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
                if (vOptionActive || VOptionActive) printf("System call made: %s\n\n", callname(orig_rax));
                addRecord(orig_rax);
            } else {
                isNotInCall = 1;
            }
          
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
       }
       printTable(); // Always have to run at the end of the process execution to show the summay of the syscalls called.
    }
    return 0;
}
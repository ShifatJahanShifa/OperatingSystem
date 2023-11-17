#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int main() {
    pid_t child_pid;
    int status;

    printf("Parent process (PID: %d) is running.\n", getpid());

    child_pid = fork();

    if (child_pid == 0) {
        // Child process
        printf("Child process (PID: %d) is running.\n", getpid());
        printf("Child process (PID: %d) will be stopped.\n", getpid());
        sleep(5);
        // Simulate being stopped by the parent
        printf("Child process (PID: %d) has been continued.\n", getpid());
        exit(0);

    } else if (child_pid > 0) {
        // Parent process
        
        // Allow some time for the child to start
        sleep(1);
        
        // Stop the child process
        kill(child_pid, SIGSTOP);
        printf("Parent process sent SIGSTOP to child (PID: %d).\n", child_pid);

        sleep(2); // Simulate some delay
        // Wait for the child to terminate
        waitpid(child_pid,&status,WUNTRACED);
        if (WIFSTOPPED(status)) {
            printf("Child process (PID: %d) was stopped and is being traced.\n", child_pid);
            kill(child_pid, SIGCONT);
            printf("Parent process sent SIGCONT to child (PID: %d).\n", child_pid);
            
            // Stop the child process
        	kill(child_pid, SIGSTOP);
        	printf("Parent process sent SIGSTOP to child (PID: %d).\n", child_pid);
        	
        	// Continue the child process
        	kill(child_pid, SIGCONT);
            printf("Parent process sent SIGCONT to child (PID: %d).\n", child_pid);
            
            sleep(2); // Simulate some delay
            waitpid(child_pid,&status,WCONTINUED);
            if (WIFCONTINUED(status)) {
                printf("Child process (PID: %d) was continued.\n", child_pid);
            }
        } 
    } else {
        perror("fork");
        exit(1);
    }

    printf("Parent process is done.\n");
    return 0;
}


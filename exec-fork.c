// Child Process
if ((pid = fork()) == 0) {
    // Move the child process to a new process group, 
    // to ensure it can handle signals seperately from the parent process
    // as in when the child process recieves a signal we don't want said signal 
    // to also be fired on the parent process, it creates bugs and sometimes 
    // infinite loops
    if (setpgid(0, 0) == -1) {
        // setpgid(0, 0) sets the process group ID of the child process
        // to its own process ID, effectively creating a new process group.
        // This is necessary to separate the child from the parent's process group,
        // allowing the child to handle signals like SIGINT independently.
        perror("setpgid failed");
        exit(EXIT_FAILURE);
    }

    // Now the child process is in its own process group, separate from the parent.
    // The child can handle signals (e.g., SIGINT from pressing Ctrl+C) independently.
    // Reset signal handling to default for the child process
    if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
        perror("Signal error in child");
        exit(EXIT_FAILURE);
    }

    char *argsv = concatenateArgs(full_command);
    printf("full_command: --%s--\n", argsv);
    free(argsv);

    // Child Process					
    execvp(command, full_command);
    printf("Command not found\n");

    // If execvp[] returns, it means it failed to execute the command.
    perror("execvp"); // Print the execvp error.
    exit(EXIT_FAILURE); // Exit the child process if execvp fails.
} else {
    // Parent Process

    // The parent process can wait for the child process to complete.
    // The parent and child are in separate groups, so signals sent to one
    // do not affect the other.
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        printf("Exit status: %d\n", WEXITSTATUS(status));
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

void change_directory(char *directory) {
    if (directory == NULL) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    } else {
        if (chdir(directory) != 0) {
            perror("cd");
        }
    }
}

void clear_screen() {
    system("clear");
}

void list_directory(char *directory) {
    if (directory == NULL) {
        directory = ".";
    }
    
    DIR *dir;
    struct dirent *entry;
    
    dir = opendir(directory);
    if (dir == NULL) {
        perror("dir");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    
    closedir(dir);
}

void list_environment() {
    extern char **environ;
    
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}

void display_comment(char *comment) {
    printf("%s\n", comment);
}

void display_help() {
    FILE *file = fopen("user_manual.txt", "r");
    if (file == NULL) {
        perror("help");
        return;
    }
    
    int c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }
    
    fclose(file);
}

void pause_shell() {
    printf("Press Enter to continue...");
    getchar();
}

void execute_program(char *args[]) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void execute_command(char *command) {
    char *args[MAX_ARGUMENTS];
    int num_args = 0;
    
    char *token = strtok(command, " ");
    while (token != NULL) {
        args[num_args] = token;
        num_args++;
        
        token = strtok(NULL, " ");
    }
    args[num_args] = NULL;
    
    if (strcmp(args[0], "cd") == 0) {
        change_directory(args[1]);
    } else if (strcmp(args[0], "clr") == 0) {
        clear_screen();
    } else if (strcmp(args[0], "dir") == 0) {
        list_directory(args[1]);
    } else if (strcmp(args[0], "environ") == 0) {
        list_environment();
    } else if (strcmp(args[0], "echo") == 0) {
        display_comment(args[1]);
    } else if (strcmp(args[0], "help") == 0) {
        display_help();
    } else if (strcmp(args[0], "pause") == 0) {
        pause_shell();
    } else if (strcmp(args[0], "quit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        execute_program(args);
    }
}

void process_input(char *input) {
    char *commands[MAX_ARGUMENTS];
    int num_commands = 0;
    
    char *token = strtok(input, "|");
    while (token != NULL) {
        commands[num_commands] = token;
        num_commands++;
        
        token = strtok(NULL, "|");
    }
    
    if (num_commands == 1) {
        execute_command(commands[0]);
    } else {
        // Implement piping and command chaining
    }
}

int main(int argc, char *argv[]) {
    char command[MAX_COMMAND_LENGTH];
    
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("batchfile");
            exit(EXIT_FAILURE);
        }
        
        while (fgets(command, sizeof(command), file)) {
            process_input(command);
        }
        
        fclose(file);
    } else {
        while (1) {
            printf("%s> ", getcwd(NULL, 0));
            fgets(command, sizeof(command), stdin);
            
            process_input(command);
        }
    }
    
    return 0;
}

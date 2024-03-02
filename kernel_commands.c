/* kernel_commands.c */

#include "kernel_commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <dirent.h>   // Library for directory handling
#include <errno.h>    // Library for error handling

/**
 * Change the current working directory to the specified path.
 */
void changeDirectory(char *path) {
    // If changing the directory fails, print an error message.
    if (chdir(path) != 0) {
        perror("chdir failed");
    }
}

/**
 * Clears the terminal screen.
 */
void clearScreen() {
    // Clear the terminal screen using the ANSI escape code.
    printf("\033[H\033[J");
}

/**
 * Outputs the given string to the terminal.
 */
void echo(char *str) {
    // Print the string followed by a new line.
    printf("%s\n", str);
}

/**
 * Prints the absolute path of the current working directory to the terminal.
 */
void printWorkingDirectory() {
    char cwd[PATH_MAX];
    // If getting the current working directory fails, print an error message.
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd failed");
    }
}


/**
 * Lists the contents of the specified directory.
 * @param path The path to the directory you want to list.
 */
void listFiles(char *path) {
    struct dirent *entry;
    DIR *dp;

    // Open the directory
    dp = opendir(path);
    if (dp == NULL) {
        perror("opendir");
        return;
    }

    // Read and print each directory entry
    while ((entry = readdir(dp)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dp);
}

/**
 * Prints all the environment variables to the terminal.
 */
void listEnviron() {
    extern char **environ;
    int i = 0;
    // Iterate through the environment variables and print each one.
    while (environ[i]) {
        printf("%s\n", environ[i++]);
    }
}

/**
 * Displays help information for the available commands.
 */
void help() {
    // Print out help information for each command.
    printf("Available commands:\n");
    printf("cd <directory> - Change the current working directory.\n");
    printf("clear - Clear the terminal screen.\n");
    printf("echo <text> - Print text to the terminal.\n");
    printf("pwd - Print the current working directory.\n");
    printf("dir <directory> - List contents of the directory.\n");
    printf("environ - List all environment variables.\n");
    printf("help - Display this help information.\n");
    printf("pause - Pause the shell operations.\n");
    printf("quit - Exit the shell.\n");
}

/**
 * Pauses the operation of the shell.
 */
void pauseShell() {
    // Prompt the user to press Enter to continue.
    printf("Press Enter to continue...");
    // Wait for the user to press Enter.
    getchar();
}

/**
 * Exits the shell.
 */
void quitShell() {
    // Perform any necessary cleanup and exit the program.
    exit(0);
}


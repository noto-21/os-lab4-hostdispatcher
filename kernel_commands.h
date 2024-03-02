/* kernel_commands.h */

/* Prevents multiple inclusions of the header file */
#ifndef KERNEL_COMMANDS_H
#define KERNEL_COMMANDS_H

/**
 * Change the current working directory to the specified path.
 * 
 * @param path A string representing the path to change to. This path can be
 * absolute (e.g., "/usr/local") or relative to the current directory (e.g., "../docs").
 */
void changeDirectory(char *path);

/**
 * Clears the terminal screen.
 * 
 * This function will send the appropriate command to the terminal to clear the screen
 * and move the cursor to the top-left position.
 */
void clearScreen();

/**
 * Outputs the given string to the terminal.
 * 
 * @param str The string to be echoed (printed out). This function will display the
 * string as is, followed by a new line.
 */
void echo(char *str);

/**
 * Prints the absolute path of the current working directory to the terminal.
 * 
 * This function queries the system for the current directory and prints it out,
 * helping the user to understand where they are in the file system.
 */
void printWorkingDirectory();

/**
 * Lists the contents of a directory.
 * 
 * @param path The path to the directory you want to list the contents of. If the
 * path is NULL or an empty string, it will list the contents of the current directory.
 */
void listFiles(char *path);

/**
 * Prints all the environment variables to the terminal.
 * 
 * This function will iterate through the system's environment variables and print
 * each one, typically in the format "KEY=value", helping users to understand the
 * environment the shell is running in.
 */
void listEnviron();

/**
 * Displays help information for the available commands.
 * 
 * This function prints out the list of available commands and a short description
 * for each, guiding users on how to use the shell and what commands are at their disposal.
 */
void help();

/**
 * Pauses the operation of the shell.
 * 
 * This function will suspend the shell's operations and wait for the user to press
 * the Enter key before continuing. It's useful for pausing the execution of scripts
 * or batch operations.
 */
void pauseShell();

/**
 * Exits the shell.
 * 
 * When this function is called, it will perform any necessary cleanup operations
 * and terminate the shell program, returning the user to their parent process or
 * closing the terminal window.
 */
void quitShell();

/* Ends the conditional inclusion of the header file */
#endif /* KERNEL_COMMANDS_H */

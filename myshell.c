#include "myshell.h"

int main(int argc, char *argv[])
{
	char cwd[PATH_MAX];
	char hostName[HOST_NAME_MAX];
	struct passwd *pw;
	uid_t uid;

	while (true) {
		// Retrieve the effective user ID of the calling process
		uid = geteuid();
		// Get the password database entry for the user ID
		pw = getpwuid(uid);

		// Print the username in green
		if (pw) {
			printf(COLOR_GREEN_BOLD "%s@" COLOR_RESET, pw->pw_name);
		}

		// Print the hostname in green
		int hostNameResult = gethostname(hostName, sizeof(hostName));
		if (hostNameResult == 0) {
			printf(COLOR_GREEN_BOLD "%s" COLOR_RESET, hostName);
		}

		// Print a colon only if both username and hostname are available
		if (pw && hostNameResult == 0) {
			printf(":");
		}

		// Print the current working directory followed by a dollar sign
		printf(COLOR_BLUE_BOLD "%s" COLOR_RESET, getcwd(cwd, sizeof(cwd)));
		printf("$ ");

		// Flush stdout to make sure all colors are reset before any other program prints to the terminal
		fflush(stdout);

		// Read a line of input, including whitespaces
		char input_char[ARG_MAX];
		fgets(input_char, sizeof(input_char), stdin);

		// Remove the newline character if it's read by fgets
		input_char[strcspn(input_char, "\n")] = 0;
		
		// Returns first token 
		char *token = strtok(trim(input_char), " ");
		char command[ARG_MAX];
		char *args[ARG_MAX];
		int i = 0;

		if (token != NULL) {
			strcpy(command, token);
		}

		// Keep printing tokens while one of the
		// delimiters present in str[].
		while (token != NULL)
		{
			// printf("%s\n", token);
			token = trim(strtok(NULL, " "));
			args[i++] = token;
		}

		
		if (strcmp(command, "cd") == 0) {
			if (args[0] != NULL) {
				changeDirectory(args[0]);
			} else if (sizeof(args) == 0) {
				changeDirectory(getenv("HOME"));
			} else {
				printf("myshell: cd: too many arguments\n");
			}
		} else if (strcmp(command, "quit") == 0) {
			quitShell();
		} else if (strcmp(command, "clr") == 0) {
			if (sizeof(args) > 0) {
				printf("Usage: clear\n");
			} else {
				clearScreen();
			}
		} else if (strcmp(command, "environ") == 0) {
			listEnviron();
		} else if (strcmp(command, "dir") == 0) {
			if (sizeof(args) > 1) {
				printf("Usage: dir [path]\n");
			} else {
				if (args[0] != NULL) {
					listFiles(args[0]);
				} else {
					listFiles(getcwd(cwd, sizeof(cwd)));
				}
			}
		} else {
			printf("Command not found\n");
			// continue;
		}
	}

	printf("\n");
	return 0;
}
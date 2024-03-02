#define _PROGRAM_NAME "myshell"
#define _AUTHOR_NAME "Okiki Ojo"

#include "kernel_commands.h"
#include "string_utils.h"

#include <stdbool.h>
#include <string.h>
#include <limits.h>		  // Used for HOST_NAME_MAX
#include <linux/limits.h> // Include the Linux-specific limits.h
#include <bits/local_lim.h>

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

// Check if ARG_MAX is defined
#ifndef ARG_MAX
#define ARG_MAX 4096 // Or a suitable default
#endif

// Define ANSI color codes with bright/bold modifiers
#define COLOR_GREEN_BOLD "\x1b[1;32m" // Bright green color
#define COLOR_BLUE_BOLD "\x1b[1;34m"  // Bright blue color
#define COLOR_RESET "\x1b[0m"		  // Reset to default terminal color

int main(int argc, char *argv[]);
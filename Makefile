CC = gcc
CFLAGS = -Wall -Wextra -g

# List of source files
SRCS = exec-fork.c process.c queue.c string_utils.c
# List of header files
HDRS = queue.h string_utils.h

# Generate object file names from source file names
OBJS = $(SRCS:.c=.o)

# The main target
dispatcher: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Rules for generating object files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f dispatcher $(OBJS)

# Clean phony
.PHONY: clean

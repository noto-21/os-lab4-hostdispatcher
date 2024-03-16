CC = gcc
CFLAGS = -Wall -Wextra -g
     
# util.c
# main.c   
# memory.c     
# queue.c    

# List of source files
SRCS = main.c queue.c memory.c util.c
# List of header files
HDRS = queue.h memory.h util.h process.h resource.h

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

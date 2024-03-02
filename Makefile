GCC = gcc
CFLAGS = -g -Wall
RM = rm -f

C_FILE = myshell.c kernel_commands.c string_utils.c
OUTPUT_EXECUTABLE = output

all: compile execute

compile: $(C_FILE)
	echo "Compiling..."
	$(GCC) $(CFLAGS) -o $(OUTPUT_EXECUTABLE) $(C_FILE) -I.
	echo "\n"

execute:
	./$(OUTPUT_EXECUTABLE)

# clean:
# 	# echo "Clean up..."
# 	# $(RM) ./$(OUTPUT_EXECUTABLE)
# 	# echo "\n"
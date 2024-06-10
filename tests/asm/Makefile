# Variables
NASM = nasm
LD = ld
LDFLAGS = -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2
TAPE_TOOL = ../../bin/tape_tool

# Paths
SRC_DIR = .
OBJ_DIR = .
BIN_DIR = ../../bin

# Files
ASM_SRC = $(SRC_DIR)/read_write_tape.asm
ASM_OBJ = $(OBJ_DIR)/read_write_tape.o
C_OBJ = $(BIN_DIR)/io.o
OUTPUT = $(OBJ_DIR)/read_write_tape
TAPE_INPUT = input.tape
TAPE_OUTPUT = output.tape

# Default target
all: $(OUTPUT) input.tape

# Link the final executable
$(OUTPUT): $(ASM_OBJ) $(C_OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)
	chmod +x $@

# Assemble the assembly file
$(ASM_OBJ): $(ASM_SRC)
	$(NASM) -f elf64 $< -o $@

# Initialize the tape
input.tape: $(TAPE_TOOL)
	$(TAPE_TOOL) new $(TAPE_INPUT) -100 100 -10 10

# Clean up the build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(OUTPUT) $(TAPE_OUTPUT)
	rm input.tape

debug: debug.gdb all
	gdb -x debug.gdb ./read_write_tape

run: all
	valgrind ./read_write_tape input.tape output.tape
	../../bin/tape_tool show output.tape

.PHONY: all clean debug run

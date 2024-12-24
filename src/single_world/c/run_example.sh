#!/bin/bash

# Get the directory of the current script
SCRIPT_DIR=$(dirname "$(realpath "$0")")

# Include directory for mpmdManager
INCLUDE_DIR="$SCRIPT_DIR/abstraction"

# Source files and output names
FILES=("first" "second" "third")
OUTPUT_DIR="$SCRIPT_DIR/example"

# Compile the .c files
for FILE in "${FILES[@]}"; do
  mpicc -I"$INCLUDE_DIR" -o "$OUTPUT_DIR/$FILE.out" "$SCRIPT_DIR/abstraction/filenameHandler.c" "$SCRIPT_DIR/abstraction/managerAuxiliar.c" "$SCRIPT_DIR/abstraction/libMPMDManager.c" "$SCRIPT_DIR/example/message.c" "$OUTPUT_DIR/$FILE.c"
done

# Build the mpirun command
RUN_COMMAND=""
for FILE in "${FILES[@]}"; do
  RUN_COMMAND="$RUN_COMMAND : -n 2 $OUTPUT_DIR/$FILE.out"
done

# Run the programs
mpirun $RUN_COMMAND

# Remove the generated .out files
rm -f "$OUTPUT_DIR/"*.out

#!/bin/bash

# Get the directory of the current script
SCRIPT_DIR=$(dirname "$(realpath "$0")")

# Include directory for mpmdManager
INCLUDE_DIR="$SCRIPT_DIR/abstraction"

# Source files and output names
FILES=("first" "second" "third")
PIDS=()
OUTPUT_DIR="$SCRIPT_DIR/example"

# Start the intermediary server and create uri.txt
ompi-server --no-daemonize -r "$SCRIPT_DIR/uri.txt" &
OMPI_SERVER_PID=$!

# Compile the .c files
for FILE in "${FILES[@]}"; do
  mpicc -I"$INCLUDE_DIR" -o "$OUTPUT_DIR/$FILE.out" "$SCRIPT_DIR/abstraction/filenameHandler.c" "$SCRIPT_DIR/abstraction/clientManager.c" "$SCRIPT_DIR/abstraction/libServerManager.c" "$SCRIPT_DIR/example/message.c" "$OUTPUT_DIR/$FILE.c"
done

# Wait for uri.txt to be created
while [ ! -f "$SCRIPT_DIR/uri.txt" ]; do
  sleep 0.1
done

# Run each program with mpirun
for FILE in "${FILES[@]}"; do
  mpirun -n 2 --ompi-server file:"$SCRIPT_DIR/uri.txt" "$SCRIPT_DIR/example/$FILE.out" &
  PIDS+=($!)
done

# Wait for all programs to finish
wait "${PIDS[@]}"

# Stop the ompi-server
kill $OMPI_SERVER_PID

# Remove the uri.txt file
rm -f "$SCRIPT_DIR/uri.txt"

# Remove the generated .out files
rm -f "$OUTPUT_DIR/"*.out

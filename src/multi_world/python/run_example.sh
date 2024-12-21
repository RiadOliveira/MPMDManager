#!/bin/bash

# Get the directory of the current script
SCRIPT_DIR=$(dirname "$(realpath "$0")")

# Set PYTHONPATH to the src/multi_world/python directory
export PYTHONPATH="$SCRIPT_DIR"

# Start the intermediary server and create uri.txt
ompi-server --no-daemonize -r "$SCRIPT_DIR/uri.txt" &
OMPI_SERVER_PID=$!

# Wait for uri.txt to be created
while [ ! -f "$SCRIPT_DIR/uri.txt" ]; do
  sleep 0.1
done

# Python scripts to execute
FILES=("first.py" "second.py" "third.py")
PIDS=()

# Run each program with mpirun
for FILE in "${FILES[@]}"; do
  mpirun -n 2 --ompi-server file:"$SCRIPT_DIR/uri.txt" python3 "$SCRIPT_DIR/example/$FILE" &
  PIDS+=($!)
done

# Wait for all programs to finish
wait "${PIDS[@]}"

# Stop the ompi-server
kill $OMPI_SERVER_PID

# Remove the uri.txt file
rm -f "$SCRIPT_DIR/uri.txt"

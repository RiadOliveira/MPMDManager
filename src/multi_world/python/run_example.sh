#!/bin/bash

# Get the directory of the current script
SCRIPT_DIR=$(dirname "$(realpath "$0")")

# Set PYTHONPATH to the src/client_server_split/python directory
export PYTHONPATH="$SCRIPT_DIR"

# Start the intermediary server and create uri.txt
ompi-server --no-daemonize -r "$SCRIPT_DIR/uri.txt" &
OMPI_SERVER_PID=$!

# Wait for uri.txt to be created
while [ ! -f "$SCRIPT_DIR/uri.txt" ]; do
  sleep 0.1
done

# Run the programs
mpirun -n 2 --ompi-server file:"$SCRIPT_DIR/uri.txt" python3 "$SCRIPT_DIR/example/first.py" &
PID_FIRST=$!

mpirun -n 2 --ompi-server file:"$SCRIPT_DIR/uri.txt" python3 "$SCRIPT_DIR/example/second.py" &
PID_SECOND=$!

mpirun -n 2 --ompi-server file:"$SCRIPT_DIR/uri.txt" python3 "$SCRIPT_DIR/example/third.py" &
PID_THIRD=$!

# Wait for all programs to finish
wait $PID_FIRST $PID_SECOND $PID_THIRD

# Stop the ompi-server
kill $OMPI_SERVER_PID

# Remove the uri.txt file
rm -f "$SCRIPT_DIR/uri.txt"

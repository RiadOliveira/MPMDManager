#!/bin/bash

# Get the directory of the current script
SCRIPT_DIR=$(dirname "$(realpath "$0")")

# Set PYTHONPATH to the src/intercomm/python directory
export PYTHONPATH="$SCRIPT_DIR"

# Run the programs
mpirun -n 2 python3 "$SCRIPT_DIR/example/first.py" : -n 2 python3 "$SCRIPT_DIR/example/second.py" : -n 2 python3 "$SCRIPT_DIR/example/third.py"

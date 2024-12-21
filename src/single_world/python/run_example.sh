#!/bin/bash

# Get the directory of the current script
SCRIPT_DIR=$(dirname "$(realpath "$0")")

# Set PYTHONPATH to the src/single_world/python directory
export PYTHONPATH="$SCRIPT_DIR"

# Source files
FILES=("first.py" "second.py" "third.py")

# Build the mpirun command
RUN_COMMAND=""
for FILE in "${FILES[@]}"; do
  RUN_COMMAND="$RUN_COMMAND : -n 2 python3 $SCRIPT_DIR/example/$FILE"
done

# Run the programs
mpirun $RUN_COMMAND

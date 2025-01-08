from mpi4py import MPI
from sys import stderr

def exit_with_error(message: str):
  stderr.write(message + "\n")
  stderr.flush()
  MPI.COMM_WORLD.Abort(1)
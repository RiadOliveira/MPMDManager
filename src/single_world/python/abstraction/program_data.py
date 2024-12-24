from mpi4py import MPI

class ProgramData:
  def __init__(self, name: str, size: int, comm: MPI.Comm):
    self.__name, self.__size, self.__comm = name, size, comm

  def disconnect_comm(self):
    if self.__comm != MPI.COMM_NULL: self.__comm.Disconnect()

  def __del__(self): self.disconnect_comm()

  @property
  def name(self): return self.__name

  @property
  def size(self): return self.__size

  @property
  def comm(self): return self.__comm
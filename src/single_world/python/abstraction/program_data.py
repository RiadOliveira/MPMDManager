from mpi4py import MPI

class ProgramData:
  def __init__(self, name: str, comm: MPI.Comm, size = 1):
    self.__name, self.__comm, self.__size = name, comm, size

  def disconnect_comm(self):
    if self.__comm != MPI.COMM_NULL: self.__comm.Disconnect()

  def __del__(self): self.disconnect_comm()

  @property
  def name(self): return self.__name

  @property
  def comm(self): return self.__comm

  @property
  def size(self): return self.__size

  def increment_size(self): self.__size += 1
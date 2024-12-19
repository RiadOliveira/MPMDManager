from mpi4py import MPI

class ProgramData:
  def __init__(self, name: str, initial_global_rank: int, comm: MPI.Comm, size: int):
    self.__name, self.__initial_global_rank, self.__comm, self.__size = name, initial_global_rank, comm, size

  def free_comm(self):
    if self.comm != MPI.COMM_NULL: self.comm.Free()

  def __del__(self): self.free_comm()

  @property
  def name(self): return self.__name

  @property
  def initial_global_rank(self): return self.__initial_global_rank

  @property
  def comm(self): return self.__comm

  @property
  def size(self): return self.__size

  def increment_size(self): self.__size += 1
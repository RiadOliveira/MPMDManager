from mpi4py import MPI

class ProgramData:
  def __init__(self, id_num: int, name: str, comm: MPI.Comm):
    self.__id_num, self.__name, self.__comm, self.__size = id_num, name, comm, 1

  def free_comm(self):
    if self.comm != MPI.COMM_NULL: self.comm.Free()

  def __del__(self): self.free_comm()

  @property
  def id_num(self): return self.__id_num

  @property
  def name(self): return self.__name

  @property
  def comm(self): return self.__comm

  @property
  def size(self): return self.__size

  def increment_size(self): self.__size += 1
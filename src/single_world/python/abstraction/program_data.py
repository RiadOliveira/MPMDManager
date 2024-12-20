from mpi4py import MPI

class ProgramData:
  __intercomm: MPI.Intercomm = MPI.COMM_NULL

  def __init__(self, id_num: int, name: str, intercomm: MPI.Intercomm):
    self.__id_num, self.__name, self.__intercomm, self.__size = id_num, name, intercomm, 1

  def disconnect_comm(self):
    if self.__intercomm != MPI.COMM_NULL: self.__intercomm.Disconnect()

  def __del__(self): self.disconnect_comm()

  @property
  def id_num(self): return self.__id_num

  @property
  def name(self): return self.__name

  @property
  def intercomm(self): return self.__intercomm

  @property
  def size(self): return self.__size

  def increment_size(self): self.__size += 1
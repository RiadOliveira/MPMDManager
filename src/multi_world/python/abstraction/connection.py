from mpi4py import MPI

class Connection:
  __comm: MPI.Intercomm

  def __init__(
    self, name: str | None = None, comm: MPI.Intercomm = MPI.COMM_NULL
  ):
    self.__name = name
    self.__comm = comm

  def __del__(self): self.disconnect_comm()

  def disconnect_comm(self):
    if(self.__comm != MPI.COMM_NULL): self.__comm.Disconnect()

  @property
  def name(self): return self.__name

  @property
  def comm(self): return self.__comm 

from mpi4py import MPI

class Connection:
  __comm: MPI.Intercomm = MPI.COMM_NULL

  def __init__(self, identifier: str, comm: MPI.Intercomm):
    self.__identifier = identifier
    self.__comm = comm

  def disconnect_comm(self):
    if(self.__comm != MPI.COMM_NULL): self.__comm.Disconnect()

  def __del__(self): self.disconnect_comm()

  @property
  def identifier(self): return self.__identifier

  @property
  def intercomm(self): return self.__comm 

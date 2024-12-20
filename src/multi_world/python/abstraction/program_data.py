from mpi4py import MPI

class ProgramData:
  __intercomm: MPI.Intercomm = MPI.COMM_NULL

  def __init__(self, identifier: str, intercomm: MPI.Intercomm):
    self.__identifier = identifier
    self.__intercomm = intercomm
  
  def disconnect_comm(self):
    if(self.__intercomm != MPI.COMM_NULL): self.__intercomm.Disconnect()

  def __del__(self): self.disconnect_comm()

  @property
  def identifier(self): return self.__identifier

  @property
  def intercomm(self): return self.__intercomm 

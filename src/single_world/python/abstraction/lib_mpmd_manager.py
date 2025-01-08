from mpi4py import MPI

from .connection import Connection
from .error import exit_with_error
from .auxiliar_data import AuxiliarConnection
from .filename_handler import get_filename

class MPMDManager:
  __connections_data: list[Connection]
  __local_ind: int
  __comm = MPI.COMM_NULL

  @staticmethod
  def init():
    MPMDManager.__comm = MPI.COMM_WORLD.Dup()
    MPMDManager.__set_connections_data()

  @staticmethod
  def finalize():
    MPMDManager.__connections_data.clear()

    manager_comm = MPMDManager.__comm
    if manager_comm != MPI.COMM_NULL: manager_comm.Disconnect()

  @staticmethod
  def local_name(): return MPMDManager.__local_connection().name

  @staticmethod
  def local_comm() -> MPI.Intracomm: 
    return MPMDManager.__local_connection().comm

  @staticmethod
  def local_rank(): return MPMDManager.local_comm().Get_rank()
  
  @staticmethod
  def local_size(): return MPMDManager.__local_connection().comm.Get_size()

  @staticmethod
  def comm_to(identifier: str | int):
    return MPMDManager.__find_connection_or_error(identifier).comm

  @staticmethod
  def size_of(identifier: str | int):
    return MPMDManager.__find_connection_or_error(identifier).size
  
  @staticmethod
  def __set_connections_data():
    world_rank = MPMDManager.__comm.Get_rank()
    world_size = MPMDManager.__comm.Get_size()

    gathered_names = MPMDManager.__comm.allgather(get_filename())
    auxiliar = MPMDManager.__get_auxiliar_data(
      gathered_names, world_rank, world_size
    )

    MPMDManager.__connections_data = [None] * len(auxiliar)
    MPMDManager.__set_local_connection_data(auxiliar, world_rank)
    MPMDManager.__set_remote_connections_data(auxiliar)

  @staticmethod
  def __get_auxiliar_data(
    gathered_names: list[str], world_rank: int, world_size: int
  ):
    data: list[AuxiliarConnection] = []

    connection_ind = -1
    previous_name: str | None = None

    for ind in range(world_size):
      current_name = gathered_names[ind]

      if previous_name == current_name: data[connection_ind].size += 1
      else:
        connection_ind += 1
        data.append(AuxiliarConnection(current_name, ind))
        if ind <= world_rank: MPMDManager.__local_ind = connection_ind

      previous_name = current_name
    
    return data

  @staticmethod
  def __set_local_connection_data(
    auxiliar: list[AuxiliarConnection], world_rank: int
  ):
    local_ind = MPMDManager.__local_ind
    local_auxiliar = auxiliar[local_ind]

    name = local_auxiliar.name
    size = local_auxiliar.size
    comm = MPMDManager.__comm.Split(local_ind, world_rank)

    connections_data = MPMDManager.__connections_data
    connections_data[local_ind] = Connection(name, size, comm)

  @staticmethod
  def __set_remote_connections_data(auxiliar: list[AuxiliarConnection]):
    manager_comm = MPMDManager.__comm
    connections_data = MPMDManager.__connections_data

    local_ind = MPMDManager.__local_ind
    local_comm: MPI.Intracomm = connections_data[local_ind].comm

    connections_quantity = len(auxiliar)
    for ind in range(connections_quantity):
      if ind == local_ind: continue

      current_auxiliar = auxiliar[ind]
      name = current_auxiliar.name
      size = current_auxiliar.size
      leader = current_auxiliar.leader

      comm = local_comm.Create_intercomm(0, manager_comm, leader)
      connections_data[ind] = Connection(name, size, comm)
  
  @staticmethod
  def __local_connection():
    return MPMDManager.__connections_data[MPMDManager.__local_ind]

  @staticmethod
  def __find_connection_or_error(identifier: str | int):
    invalid_identifier = not isinstance(identifier, (str, int))
    if invalid_identifier: exit_with_error("Invalid connection id!")

    connection_found = MPMDManager.__find_connection(identifier)
    if connection_found is None: exit_with_error("Connection not found!")

    return connection_found

  @staticmethod
  def __find_connection(identifier: str | int):
    is_ind = isinstance(identifier, int)

    if is_ind: return MPMDManager.__find_connection_by_ind(identifier)
    return MPMDManager.__find_connection_by_name(identifier)
  
  @staticmethod
  def __find_connection_by_ind(ind: int):
    invalid_ind = ind < 0 or ind >= len(MPMDManager.__connections_data)
    return None if invalid_ind else MPMDManager.__connections_data[ind]
  
  @staticmethod
  def __find_connection_by_name(name: str):
    for data in MPMDManager.__connections_data: 
      if name == data.name: return data

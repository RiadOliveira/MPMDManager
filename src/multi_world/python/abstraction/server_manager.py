from mpi4py import MPI

from .manager import Manager
from .connection import Connection
from .filename_handler import get_filename

class ServerManager(Manager):
  __comm = MPI.COMM_NULL
  __port_name = ""

  @staticmethod
  def init(server_name: str | None = None):
    ServerManager.__comm = MPI.COMM_SELF.Dup()
    ServerManager.__name = ServerManager.__parse_name(server_name)

  @staticmethod
  def finalize():
    ServerManager.disconnect_clients()

    manager_comm = ServerManager.__comm
    if manager_comm != MPI.COMM_NULL: manager_comm.Disconnect()
    
    ServerManager.close()

  @staticmethod
  def open():
    ServerManager.__port_name = MPI.Open_port()
    MPI.Publish_name(ServerManager.__name, ServerManager.__port_name)

  @staticmethod
  def close():
    MPI.Unpublish_name(ServerManager.__name, ServerManager.__port_name)
    MPI.Close_port(ServerManager.__port_name)

  @staticmethod
  def accept(client_id: str | None = None):
    Manager._validate_connection_addition(client_id)

    client_comm = ServerManager.__comm.Accept(ServerManager.__port_name)
    
    client_added = Connection(client_id, client_comm)
    Manager._connections.append(client_added)
    return client_added.comm
  
  @staticmethod
  def retrieve_client_comm(client_id: str | int):
    return Manager._find_connection_or_error(client_id).comm
  
  @staticmethod
  def disconnect_client(client_id: str | int):
    ind_found = Manager._find_connection_ind_or_error(client_id)
    Manager._connections.pop(ind_found)

  @staticmethod
  def disconnect_clients(): Manager._connections.clear()

  @staticmethod
  def name(): return ServerManager.__name

  @staticmethod
  def __parse_name(name: str | None):
    if name != None: return name

    rank = MPI.COMM_WORLD.Get_rank()
    return get_filename() + str(rank)

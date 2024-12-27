from mpi4py import MPI

from .manager import Manager
from .connection import Connection
from .filename_handler import get_filename

class ServerManager:
  __comm = MPI.COMM_NULL
  __clients: list[Connection] = []

  @staticmethod
  def init(server_name = ""):
    ServerManager.__comm = MPI.COMM_SELF.Dup()
    ServerManager.__name = ServerManager.__parse_name(server_name)

  @staticmethod
  def open():
    ServerManager.__port_name = MPI.Open_port()
    MPI.Publish_name(ServerManager.__name, ServerManager.__port_name)

  @staticmethod
  def accept(client_id = ""):
    parsed_identifier = ServerManager.__parse_client_id(
      client_id
    )
    client_intercomm = ServerManager.__comm.Accept(
      ServerManager.__port_name
    )
    
    client_data = Connection(parsed_identifier, client_intercomm)
    ServerManager.__clients.append(client_data)
    return client_intercomm
  
  @staticmethod
  def retrieve_client_comm(client_id: str | int):
    return Manager.retrieve_connection_comm(
      client_id, ServerManager.__clients
    )
  
  @staticmethod
  def disconnect_clients(): ServerManager.__clients.clear()

  @staticmethod
  def finalize():
    ServerManager.disconnect_clients()
    ServerManager.close()

    if ServerManager.__comm != MPI.COMM_NULL:
      ServerManager.__comm.Disconnect()

  @staticmethod
  def close():
    MPI.Unpublish_name(ServerManager.__name, ServerManager.__port_name)
    MPI.Close_port(ServerManager.__port_name)

  @staticmethod
  def name(): return ServerManager.__name

  @staticmethod
  def __parse_name(name: str):
    if name != '': return name

    rank = MPI.COMM_WORLD.Get_rank()
    return get_filename() + str(rank)
  
  def __parse_client_id(identifier: str):
    if identifier != '': return identifier
    return str(len(ServerManager.__clients))

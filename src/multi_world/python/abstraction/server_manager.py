from mpi4py import MPI

from .manager import Manager
from .program_data import ProgramData
from .filename_handler import get_filename

class ServerManager:
  __manager_comm: MPI.Intracomm = MPI.COMM_NULL
  __clients_data: list[ProgramData] = []

  @staticmethod
  def initialize(server_name = ""):
    ServerManager.__manager_comm = MPI.COMM_SELF.Dup()
    ServerManager.__server_name = ServerManager.__parse_server_name(
      server_name
    )

  @staticmethod
  def open():
    ServerManager.__port_name = MPI.Open_port()
    MPI.Publish_name(ServerManager.__server_name, ServerManager.__port_name)

  @staticmethod
  def accept(client_identifier = ""):
    parsed_identifier = ServerManager.__parse_client_identifier(
      client_identifier
    )
    client_intercomm = ServerManager.__manager_comm.Accept(
      ServerManager.__port_name
    )
    
    client_data = ProgramData(parsed_identifier, client_intercomm)
    ServerManager.__clients_data.append(client_data)
    return client_intercomm
  
  @staticmethod
  def intercomm_to_connected_client(client_identifier: str | int):
    return Manager.intercomm_to_connected_program(
      client_identifier, ServerManager.__clients_data
    )
  
  @staticmethod
  def disconnect_clients(): ServerManager.__clients_data.clear()

  @staticmethod
  def finalize():
    ServerManager.disconnect_clients()
    ServerManager.close()

    if ServerManager.__manager_comm != MPI.COMM_NULL:
      ServerManager.__manager_comm.Disconnect()

  @staticmethod
  def close():
    MPI.Unpublish_name(ServerManager.__server_name, ServerManager.__port_name)
    MPI.Close_port(ServerManager.__port_name)

  @staticmethod
  def name(): return ServerManager.__server_name

  @staticmethod
  def __parse_server_name(name: str):
    if name != '': return name

    rank = MPI.COMM_WORLD.Get_rank()
    return get_filename() + str(rank)
  
  def __parse_client_identifier(identifier: str):
    if identifier != '': return identifier
    return str(len(ServerManager.__clients_data))

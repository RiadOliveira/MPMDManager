from mpi4py import MPI

from .manager import Manager
from .program_data import ProgramData
from .filename_handler import get_filename

class ServerManager:
  __port_name: str
  __clients_data: list[ProgramData] = []

  @staticmethod
  def open(server_name = ""):
    ServerManager.__server_name = ServerManager.__parse_name(
      server_name
    )

    ServerManager.__port_name = MPI.Open_port()
    MPI.Publish_name(ServerManager.__server_name, ServerManager.__port_name)

  @staticmethod
  def accept(client_identifier = ""):
    client_comm = MPI.COMM_SELF.Accept(ServerManager.__port_name)
    
    index_identifier = str(len(ServerManager.__clients_data))
    parsed_identifier = client_identifier if client_identifier != "" else index_identifier

    ServerManager.__clients_data.append(ProgramData(parsed_identifier, client_comm))
    return client_comm
  
  @staticmethod
  def comm_to_already_connected_client(
    client_identifier: str | int
  ):
    return Manager.comm_to_already_connected_program(
      client_identifier, ServerManager.__clients_data
    )

  @staticmethod
  def finalize():
    ServerManager.disconnect_clients()
    ServerManager.close()

  @staticmethod
  def disconnect_clients():
    ServerManager.__disconnect_client_comms()
    ServerManager.__clients_data.clear()

  @staticmethod
  def close():
    MPI.Unpublish_name(ServerManager.__server_name, ServerManager.__port_name)
    MPI.Close_port(ServerManager.__port_name)

  @property
  @staticmethod
  def name(): return ServerManager.__server_name

  @staticmethod
  def __parse_name(received_name: str):
    if(received_name != ""): return received_name

    program_name = get_filename()
    rank = MPI.COMM_WORLD.Get_rank()
    return program_name + str(rank)
  
  @staticmethod
  def __disconnect_client_comms():
    for client_data in ServerManager.__clients_data:
      client_data.disconnect_comm()


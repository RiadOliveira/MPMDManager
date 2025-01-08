from mpi4py import MPI
from time import sleep

from .manager import Manager
from .connection import Connection
from .error import exit_with_error
from .connect_attempt_data import ConnectAttemptData

class ClientManager(Manager):
  __comm = MPI.COMM_NULL

  @staticmethod
  def init(): ClientManager.__comm = MPI.COMM_SELF.Dup()

  @staticmethod
  def finalize(): 
    ClientManager.disconnect_servers()

    manager_comm = ClientManager.__comm
    if manager_comm != MPI.COMM_NULL: manager_comm.Disconnect()

  @staticmethod
  def connect(server_name: str, attemptData = ConnectAttemptData()):
    Manager._validate_connection_addition(server_name)

    port_name = ClientManager.__attempt_server_port_name_lookup(
      server_name, attemptData
    )
    server_comm = ClientManager.__comm.Connect(port_name)

    server_added = Connection(server_name, server_comm)
    Manager._connections.append(server_added)
    return server_added.comm

  @staticmethod
  def retrieve_server_comm(server_id: str | int):
    return Manager._find_connection_or_error(server_id).comm
  
  @staticmethod
  def disconnect_server(server_id: str | int):
    ind_found = Manager._find_connection_ind_or_error(server_id)
    Manager._connections.pop(ind_found)

  @staticmethod
  def disconnect_servers(): Manager._connections.clear()

  @staticmethod
  def __attempt_server_port_name_lookup(
    server_name: str, data: ConnectAttemptData
  ):
    max_attempts = data.max_attempts

    for attempts in range(max_attempts):
      try:
        port_name = MPI.Lookup_name(server_name)
        return port_name
      except:
        wait_time_ms = ClientManager.__get_current_wait_time(data, attempts)
        ClientManager.__sleep_ms(wait_time_ms)

    exit_with_error(
      f"Failed to lookup port name for server '{server_name}' after {max_attempts} attempts!"
    )
  
  @staticmethod
  def __get_current_wait_time(data: ConnectAttemptData, attempts: int):
    return min(
      data.max_wait_ms, data.initial_wait_ms + attempts * data.wait_increment_ms
    )
  
  def __sleep_ms(milliseconds: int): sleep(milliseconds / 1000)

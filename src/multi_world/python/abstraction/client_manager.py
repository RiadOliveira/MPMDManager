from mpi4py import MPI
from time import sleep

from .manager import Manager
from .connection import Connection
from .request_attempts_data import RequestAttemptsData

class ClientManager:
  __comm = MPI.COMM_NULL
  __servers: list[Connection] = []

  @staticmethod
  def init(): ClientManager.__comm = MPI.COMM_SELF.Dup()

  @staticmethod
  def request(server_name: str, attempts_data = RequestAttemptsData()):
    port_name = ClientManager.__try_to_connect(server_name, attempts_data)
    server_intercomm = ClientManager.__comm.Connect(port_name)

    server_data = Connection(server_name, server_intercomm)
    ClientManager.__servers.append(server_data)
    return server_intercomm

  @staticmethod
  def retrieve_server_comm(server_id: str | int):
    return Manager.retrieve_connection_comm(
      server_id, ClientManager.__servers
    )
  
  @staticmethod
  def disconnect_servers(): ClientManager.__servers.clear()

  @staticmethod
  def finalize(): 
    ClientManager.disconnect_servers()

    if ClientManager.__comm != MPI.COMM_NULL: ClientManager.__comm.Disconnect()

  @staticmethod
  def __try_to_connect(
    server_name: str, attempts_data: RequestAttemptsData
  ):
    attempts = 0
    max_attempts = attempts_data.max_attempts_to_connect
    initial_wait_time = attempts_data.initial_attempts_wait_time_in_secs
    max_attempts_wait_time = attempts_data.max_attempts_wait_time_in_secs
    
    while(attempts < max_attempts):
      try:
        port_name = MPI.Lookup_name(server_name)
        return port_name
      except:
        backoff = min(
          initial_wait_time * 2 ** attempts, max_attempts_wait_time
        )
        attempts += 1
        sleep(backoff)

    raise RuntimeError(
      f"Failed to get port name from server after {max_attempts} attempts!"
    )

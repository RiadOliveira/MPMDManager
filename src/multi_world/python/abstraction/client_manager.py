from time import sleep
from mpi4py import MPI
from .manager import Manager
from .program_data import ProgramData
from .request_attempts_data import RequestAttemptsData

class ClientManager:
  __servers_data: list[ProgramData] = []

  @staticmethod
  def request(
    server_name: str, attempts_data = RequestAttemptsData()
  ):
    port_name = ClientManager.__try_to_connect(server_name, attempts_data)
    server_comm = MPI.COMM_SELF.Connect(port_name)

    ClientManager.__servers_data.append(ProgramData(server_name, server_comm))
    return server_comm

  @staticmethod
  def comm_to_already_connected_server(
    server_identifier: str | int
  ):
    return Manager.comm_to_already_connected_program(
      server_identifier, ClientManager.__servers_data
    )
  
  @staticmethod
  def finalize(): ClientManager.disconnect_servers()
  
  @staticmethod
  def disconnect_servers():
    ClientManager.__disconnect_server_comms()
    ClientManager.__servers_data.clear()

  @staticmethod
  def __disconnect_server_comms():
    for servers_data in ClientManager.__servers_data:
      servers_data.disconnect_comm()

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
      f"Failed to get port name from server after {max_attempts} attempts"
    )

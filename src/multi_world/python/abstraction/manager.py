from abc import ABCMeta

from .connection import Connection
from .error import exit_with_error

class Manager(ABCMeta):
  _connections: list[Connection] = []

  @staticmethod
  def _validate_connection_addition(name: str | None):
    if name == None: return

    connection_found = Manager.__find_connection_by_name(name)
    if connection_found != None:
      exit_with_error(f"A connection named '{name}' already exists!")

  @staticmethod
  def _find_connection_or_error(connection_id: str | int):
    invalid_id = not isinstance(connection_id, (str, int))
    if invalid_id: exit_with_error("Invalid connection id!")

    connection_found = Manager.__find_connection(connection_id)
    if connection_found is None: exit_with_error("Connection not found!")

    return connection_found

  @staticmethod
  def _find_connection_ind_or_error(connection_id: str | int):
    invalid_id = not isinstance(connection_id, (str, int))
    if invalid_id: exit_with_error("Invalid connection id!")

    ind = Manager.__find_connection_ind(connection_id)
    if ind is None: exit_with_error("Connection not found!")

    return ind

  @staticmethod
  def __find_connection(connection_id: str | int):
    name_id = not isinstance(connection_id, int)

    if name_id: return Manager.__find_connection_by_name(connection_id)
    return Manager.__find_connection_by_ind(connection_id)

  @staticmethod
  def __find_connection_by_ind(ind: int):
    connections = Manager._connections
    return connections[ind] if Manager.__valid_ind(ind) else None
  
  @staticmethod
  def __find_connection_by_name(name: str):
    connections = Manager._connections

    for data in connections: 
      if name == data.name: return data
  
  @staticmethod
  def __find_connection_ind(connection_id: str | int):
    name_id = not isinstance(connection_id, int)

    if name_id: return Manager.__find_connection_ind_by_name(connection_id)
    return connection_id if Manager.__valid_ind(connection_id) else None

  @staticmethod
  def __find_connection_ind_by_name(name: str):
    connections = Manager._connections

    for ind, data in enumerate(connections):
      if name == data.name: return ind

  @staticmethod
  def __valid_ind(ind: int): return ind >= 0 and ind < len(Manager._connections)
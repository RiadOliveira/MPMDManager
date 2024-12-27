from abc import ABCMeta

from .connection import Connection

class Manager(ABCMeta):
  @staticmethod
  def retrieve_connection_comm(
    connection_id: str | int, connections: list[Connection]
  ):
    invalid_identifier = not isinstance(connection_id, (str, int))
    if(invalid_identifier): raise Exception("Invalid connection id!")

    connection_found = Manager.__find_connection(connection_id, connections)
    if(connection_found is None): raise Exception("Connection not found!")

    return connection_found.comm

  @staticmethod
  def __find_connection(
    identifier: str | int, connections: list[Connection]
  ):
    is_ind = isinstance(identifier, int)

    if is_ind: return Manager.__find_connection_by_ind(identifier, connections)
    return Manager.__find_connection_by_name(identifier, connections)

  @staticmethod
  def __find_connection_by_ind(ind: int, connections: list[Connection]):
    invalid_ind = ind < 0 or ind >= len(connections)
    return None if invalid_ind else connections[ind]
  
  @staticmethod
  def __find_connection_by_name(name: str, connections: list[Connection]):
    for data in connections: 
      if name == data.name: return data
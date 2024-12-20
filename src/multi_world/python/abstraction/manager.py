from abc import ABCMeta
from .program_data import ProgramData

class Manager(ABCMeta):
  @staticmethod
  def comm_to_connected_program(
    program_identifier: str | int, programs_data: list[ProgramData]
  ):
    invalid_identifier = not isinstance(program_identifier, (str, int))
    if(invalid_identifier): raise Exception("Invalid program identifier")

    program_found = Manager.__find_program(program_identifier, programs_data)
    if(program_found is None): raise Exception("Invalid program identifier")

    return program_found.intercomm

  @staticmethod
  def __find_program(
    identifier: str | int, programs_data: list[ProgramData]
  ):
    if isinstance(identifier, int):
      invalid_index = identifier < 0 or identifier >= len(programs_data)
      return None if invalid_index else programs_data[identifier]
    
    return next(
      (data for data in programs_data if identifier == data.identifier),
      None
    )
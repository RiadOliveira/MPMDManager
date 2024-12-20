from abc import ABCMeta

from .program_data import ProgramData

class Manager(ABCMeta):
  @staticmethod
  def intercomm_to_connected_program(
    program_identifier: str | int, programs_data: list[ProgramData]
  ):
    invalid_identifier = not isinstance(program_identifier, (str, int))
    if(invalid_identifier): raise Exception("Invalid program identifier!")

    program_found = Manager.__find_program(program_identifier, programs_data)
    if(program_found is None): raise Exception("Program not found!")

    return program_found.intercomm

  @staticmethod
  def __find_program(
    identifier: str | int, programs_data: list[ProgramData]
  ):
    is_ind = isinstance(identifier, int)

    if is_ind: return Manager.__find_program_by_ind(identifier, programs_data)
    return Manager.__find_program_by_name(identifier, programs_data)

  @staticmethod
  def __find_program_by_ind(ind: int, programs_data: list[ProgramData]):
    invalid_ind = ind < 0 or ind >= len(programs_data)
    return None if invalid_ind else programs_data[ind]
  
  @staticmethod
  def __find_program_by_name(name: str, programs_data: list[ProgramData]):
    for data in programs_data: 
      if name == data.name: return data
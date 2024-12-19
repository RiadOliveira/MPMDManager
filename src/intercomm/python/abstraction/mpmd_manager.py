from mpi4py import MPI
from sys import argv as program_args

from .program_data import ProgramData
from .filename_handler import get_filename_from_path

class MPMDManager:
  __local_data: ProgramData
  __manager_comm = MPI.COMM_NULL
  __programs_data: list[ProgramData] = []

  @staticmethod
  def initialize():
    MPMDManager.__manager_comm = MPI.COMM_WORLD.Dup()
  
    local_name = get_filename_from_path(program_args[0])
    gathered_names = MPMDManager.__manager_comm.allgather(
      local_name
    )

    MPMDManager.__set_local_program_data(gathered_names, local_name)
    MPMDManager.__fill_programs_data(gathered_names)

  @staticmethod
  def local_comm() -> MPI.Intracomm: return MPMDManager.__local_data.comm
  
  @staticmethod
  def local_size(): return MPMDManager.__local_data.size

  @staticmethod
  def comm_to(program_identifier: str | int):
    return MPMDManager.__find_program_or_raise_exception(
      program_identifier
    ).comm

  @staticmethod
  def size_of(program_identifier: str | int):
    return MPMDManager.__find_program_or_raise_exception(
      program_identifier
    ).size

  @staticmethod
  def finalize():
    for program_data in MPMDManager.__programs_data: program_data.free_comm()
    MPMDManager.__programs_data.clear()

    if MPMDManager.__manager_comm != MPI.COMM_NULL:
      MPMDManager.__manager_comm.Free()

  @staticmethod
  def __set_local_program_data(gathered_names: list[str], local_name: str):
    global_rank = MPMDManager.__manager_comm.Get_rank()

    ind = global_rank - 1
    while ind >= 0 and gathered_names[ind] == local_name: ind -= 1
    local_initial_rank = ind + 1

    comm = MPMDManager.__manager_comm.Split(
      local_initial_rank, global_rank
    )
    MPMDManager.__local_data = ProgramData(
      local_name, local_initial_rank, comm, comm.Get_size()
    )
  
  @staticmethod
  def __fill_programs_data(gathered_names: list[str | None]):
    previous_name: str | None = None
    local_data = MPMDManager.__local_data
    length = len(gathered_names)

    rank = 0
    while rank < length:
      is_local_program = rank == local_data.initial_global_rank
      if is_local_program:
        MPMDManager.__programs_data.append(local_data)
        rank += local_data.size
        continue

      name = gathered_names[rank]
      if name == previous_name: MPMDManager.__programs_data[-1].increment_size()
      else:
        previous_name = name
        intercomm = MPMDManager.__get_program_intercomm(rank)
        MPMDManager.__programs_data.append(ProgramData(name, rank, intercomm, 1))
      
      rank += 1

  @staticmethod
  def __get_program_intercomm(remote_initial_global_rank: int):
    return MPMDManager.__local_data.comm.Create_intercomm(
      0, MPMDManager.__manager_comm, remote_initial_global_rank, 0
    )
  
  @staticmethod
  def __find_program_or_raise_exception(program_identifier: str | int):
    invalid_identifier = not isinstance(program_identifier, (str, int))
    if invalid_identifier: raise Exception("Invalid program identifier")

    program_found = MPMDManager.__find_program(program_identifier)
    if program_found is None: raise Exception("Invalid program identifier")

    return program_found

  @staticmethod
  def __find_program(identifier: str | int):
    if isinstance(identifier, int):
      invalid_index = identifier < 0 or identifier >= len(MPMDManager.__programs_data)
      return None if invalid_index else MPMDManager.__programs_data[identifier]
    
    for data in MPMDManager.__programs_data: 
      if identifier.startswith(data.name): return data

    return None

from mpi4py import MPI

from .program_data import ProgramData
from .filename_handler import get_filename

class MPMDManager:
  __local_data: ProgramData
  __manager_comm = MPI.COMM_NULL
  __programs_data: list[ProgramData] = []

  @staticmethod
  def initialize():
    MPMDManager.__manager_comm = MPI.COMM_WORLD.Dup()

    local_name = get_filename()
    gathered_names = MPMDManager.__manager_comm.allgather(
      local_name
    )
    MPMDManager.__set_programs_data(local_name, gathered_names)

  @staticmethod
  def local_name(): return MPMDManager.__local_data.name

  @staticmethod
  def local_comm() -> MPI.Intracomm: return MPMDManager.__local_data.comm
  
  @staticmethod
  def local_size(): return MPMDManager.__local_data.comm.Get_size()

  @staticmethod
  def intercomm_to(identifier: str | int):
    return MPMDManager.__find_program_or_error(identifier).comm

  @staticmethod
  def size_of(identifier: str | int):
    return MPMDManager.__find_program_or_error(identifier).size

  @staticmethod
  def finalize():
    MPMDManager.__programs_data.clear()

    local_comm = MPMDManager.local_comm()
    if local_comm != MPI.COMM_NULL: local_comm.Disconnect()

    manager_comm = MPMDManager.__manager_comm
    if manager_comm != MPI.COMM_NULL: manager_comm.Disconnect()
  
  @staticmethod
  def __set_programs_data(local_name: str, gathered_names: list[str]):
    MPMDManager.__set_local_data(local_name, gathered_names)

    previous_name: str | None = None
    rank, length = 0, len(gathered_names)

    while rank < length:
      name = gathered_names[rank]
      rank += 1

      if name == previous_name: 
        MPMDManager.__programs_data[-1].increment_size()
      else:
        previous_name = name

        if name == local_name: 
          MPMDManager.__programs_data.append(MPMDManager.__local_data)
          rank += MPMDManager.local_size() - 1
        else: MPMDManager.__add_program(name, rank - 1)          

  @staticmethod
  def __set_local_data(local_name: str, gathered_names: list[str]):
    global_rank = MPMDManager.__manager_comm.Get_rank()
    local_id = gathered_names.index(local_name)

    local_comm = MPMDManager.__manager_comm.Split(local_id, global_rank)
    local_size = local_comm.Get_size()
    MPMDManager.__local_data = ProgramData(local_name, local_comm, local_size)

  @staticmethod
  def __add_program(name: str, initial_rank: int):
    intercomm = MPMDManager.__get_program_intercomm(initial_rank)
    MPMDManager.__programs_data.append(ProgramData(name, intercomm))

  
  @staticmethod
  def __get_program_intercomm(remote_program_initial_rank: int):
    return MPMDManager.local_comm().Create_intercomm(
      0, MPMDManager.__manager_comm, remote_program_initial_rank
    )
  
  @staticmethod
  def __find_program_or_error(identifier: str | int):
    invalid_identifier = not isinstance(identifier, (str, int))
    if invalid_identifier: raise Exception("Invalid program identifier!")

    program_found = MPMDManager.__find_program(identifier)
    if program_found is None: raise Exception("Program not found!")

    return program_found

  @staticmethod
  def __find_program(identifier: str | int):
    is_ind = isinstance(identifier, int)

    if is_ind: return MPMDManager.__find_program_by_ind(identifier)
    return MPMDManager.__find_program_by_name(identifier)
  
  @staticmethod
  def __find_program_by_ind(ind: int):
    invalid_ind = ind < 0 or ind >= len(MPMDManager.__programs_data)
    return None if invalid_ind else MPMDManager.__programs_data[ind]
  
  @staticmethod
  def __find_program_by_name(name: str):
    for data in MPMDManager.__programs_data: 
      if name == data.name: return data

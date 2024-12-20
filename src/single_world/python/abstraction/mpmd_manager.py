from mpi4py import MPI

from .program_data import ProgramData
from .filename_handler import get_filename

class MPMDManager:
  __local_comm = MPI.COMM_NULL
  __manager_comm = MPI.COMM_NULL
  __programs_data: list[ProgramData] = []

  @staticmethod
  def initialize():
    MPMDManager.__manager_comm = MPI.COMM_WORLD.Dup()

    local_id = MPMDManager.__manager_comm.Get_attr(MPI.APPNUM)
    MPMDManager.__set_local_comm(local_id)

    local_rank = MPMDManager.__local_comm.Get_rank()
    name_data = None if local_rank != 0 else get_filename()

    gathered_names = MPMDManager.__manager_comm.allgather(
      name_data
    )
    MPMDManager.__fill_programs_data(local_id, gathered_names)

  @staticmethod
  def local_name(): return MPMDManager.__local_name

  @staticmethod
  def local_comm() -> MPI.Intracomm: return MPMDManager.__local_comm
  
  @staticmethod
  def local_size(): return MPMDManager.__local_comm.Get_size()

  @staticmethod
  def intercomm_to(identifier: str | int):
    return MPMDManager.__find_program_or_raise_exception(
      identifier
    ).intercomm

  @staticmethod
  def size_of(identifier: str | int):
    return MPMDManager.__find_program_or_raise_exception(
      identifier
    ).size

  @staticmethod
  def finalize():
    MPMDManager.__programs_data.clear()

    if MPMDManager.__local_comm != MPI.COMM_NULL:
      MPMDManager.__local_comm.Disconnect()

    if MPMDManager.__manager_comm != MPI.COMM_NULL:
      MPMDManager.__manager_comm.Disconnect()

  @staticmethod
  def __set_local_comm(local_id: int):
    global_rank = MPMDManager.__manager_comm.Get_rank()

    MPMDManager.__local_comm = MPMDManager.__manager_comm.Split(
      local_id, global_rank
    )
  
  @staticmethod
  def __fill_programs_data(local_id: int, gathered_names: list[str | None]):
    current_program: ProgramData | None = None

    length = len(gathered_names)
    rank, id_num = 0, -1
    
    while rank < length:
      name = gathered_names[rank]

      if name == None: current_program.increment_size()
      else:
        id_num += 1
        
        if id_num == local_id: 
          MPMDManager.__local_name = name
          rank += MPMDManager.local_size() - 1
        else: current_program = MPMDManager.__add_program(id_num, name, rank)
          
      rank += 1

  @staticmethod
  def __add_program(id_num: int, name: str, initial_rank: int):
    intercomm = MPMDManager.__get_program_intercomm(initial_rank)
    current_program = ProgramData(id_num, name, intercomm)
    MPMDManager.__programs_data.append(current_program)

    return current_program

  @staticmethod
  def __get_program_intercomm(remote_program_initial_rank: int):
    return MPMDManager.__local_comm.Create_intercomm(
      0, MPMDManager.__manager_comm, remote_program_initial_rank, 0
    )
  
  @staticmethod
  def __find_program_or_raise_exception(identifier: str | int):
    invalid_identifier = not isinstance(identifier, (str, int))
    if invalid_identifier: raise Exception("Invalid program identifier!")

    program_found = MPMDManager.__find_program(identifier)
    if program_found is None: raise Exception("Program not found!")

    return program_found

  @staticmethod
  def __find_program(identifier: str | int):
    is_id_num = isinstance(identifier, int)

    if is_id_num: return MPMDManager.__find_program_by_id_num(identifier)
    return MPMDManager.__find_program_by_name(identifier)
  
  @staticmethod
  def __find_program_by_id_num(id_num: int):
    for data in MPMDManager.__programs_data: 
      if id_num == data.id_num: return data
  
  @staticmethod
  def __find_program_by_name(name: str):
    for data in MPMDManager.__programs_data: 
      if name == data.name: return data

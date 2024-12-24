from mpi4py import MPI

from .program_data import ProgramData
from .auxiliar_data import AuxiliarProgramData
from .filename_handler import get_filename

class MPMDManager:
  __programs_data: list[ProgramData]
  __local_program_ind: int
  __comm = MPI.COMM_NULL

  @staticmethod
  def init():
    MPMDManager.__comm = MPI.COMM_WORLD.Dup()
    MPMDManager.__set_programs_data()

  @staticmethod
  def finalize():
    MPMDManager.__programs_data.clear()

    manager_comm = MPMDManager.__comm
    if manager_comm != MPI.COMM_NULL: manager_comm.Disconnect()

  @staticmethod
  def local_name(): return MPMDManager.__local_program().name

  @staticmethod
  def local_comm() -> MPI.Intracomm: return MPMDManager.__local_program().comm

  @staticmethod
  def local_rank(): return MPMDManager.local_comm().Get_rank()
  
  @staticmethod
  def local_size(): return MPMDManager.__local_program().comm.Get_size()

  @staticmethod
  def intercomm_to(identifier: str | int):
    return MPMDManager.__find_program_or_error(identifier).comm

  @staticmethod
  def size_of(identifier: str | int):
    return MPMDManager.__find_program_or_error(identifier).size
  
  @staticmethod
  def __set_programs_data():
    world_rank = MPMDManager.__comm.Get_rank()
    world_size = MPMDManager.__comm.Get_size()

    gathered_names = MPMDManager.__comm.allgather(get_filename())
    auxiliar = MPMDManager.__get_auxiliar_data(
      gathered_names, world_rank, world_size
    )

    MPMDManager.__programs_data = [None] * len(auxiliar)
    MPMDManager.__set_local_program_data(auxiliar, world_rank)
    MPMDManager.__set_remote_programs_data(auxiliar)

  @staticmethod
  def __get_auxiliar_data(
    gathered_names: list[str], world_rank: int, world_size: int
  ):
    data: list[AuxiliarProgramData] = []

    program_ind = -1
    previous_name: str | None = None

    for ind in range(world_size):
      current_name = gathered_names[ind]

      if previous_name == current_name: data[program_ind].size += 1
      else:
        program_ind += 1
        data.append(AuxiliarProgramData(current_name, ind))
        if ind <= world_rank: MPMDManager.__local_program_ind = program_ind

      previous_name = current_name
    
    return data

  @staticmethod
  def __set_local_program_data(
    auxiliar: list[AuxiliarProgramData], world_rank: int
  ):
    local_ind = MPMDManager.__local_program_ind
    local_auxiliar = auxiliar[local_ind]

    name = local_auxiliar.name
    size = local_auxiliar.size
    comm = MPMDManager.__comm.Split(local_ind, world_rank)

    programs_data = MPMDManager.__programs_data
    programs_data[local_ind] = ProgramData(name, size, comm)

  @staticmethod
  def __set_remote_programs_data(auxiliar: list[AuxiliarProgramData]):
    manager_comm = MPMDManager.__comm
    programs_data = MPMDManager.__programs_data

    local_ind = MPMDManager.__local_program_ind
    local_comm: MPI.Intracomm = programs_data[local_ind].comm

    programs_quantity = len(auxiliar)
    for ind in range(programs_quantity):
      if ind == local_ind: continue

      current_auxiliar = auxiliar[ind]
      name = current_auxiliar.name
      size = current_auxiliar.size
      leader = current_auxiliar.leader

      comm = local_comm.Create_intercomm(0, manager_comm, leader)
      programs_data[ind] = ProgramData(name, size, comm)
  
  @staticmethod
  def __local_program():
    return MPMDManager.__programs_data[MPMDManager.__local_program_ind]

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

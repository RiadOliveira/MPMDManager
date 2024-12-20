from mpi4py import MPI
from abstraction import ServerManager, ClientManager

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
  ServerManager.initialize()
  ServerManager.open()

  third_comm = ServerManager.accept()
  transfer_object = {'name': '0|Second to 1|Third', 'value': 2031}

  # Send data to 1|Third, mapped as rank 0 (dest=0) in third_comm, 
  # since it's the only process in this communicator
  third_comm.send(transfer_object, 0)
  
  print('(Send) 0|Second -> 1|Third: ', end=' ')
  print(transfer_object)

  ServerManager.finalize()
elif rank == 1:
  ClientManager.initialize()

  first_comm = ClientManager.request('first0')
  received_object = first_comm.recv(source=0)
  
  print('(Recv) 1|Second <- 0|First: ', end=' ')
  print(received_object)

  ClientManager.finalize()

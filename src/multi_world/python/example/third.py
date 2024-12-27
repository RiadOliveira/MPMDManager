from mpi4py import MPI
from abstraction import ServerManager, ClientManager

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
  ServerManager.init()
  ServerManager.open()

  first_comm = ServerManager.accept()
  transfer_object = {'name': '0|Third  to 1|First', 'value': 3011}

  # Send data to 1|First, mapped as rank 0 (dest=0) in first_comm, 
  # since it's the only process in this communicator
  first_comm.send(transfer_object, 0)
  
  print('(Send) 0|Third  -> 1|First: ', end=' ')
  print(transfer_object)

  ServerManager.finalize()
elif rank == 1:
  ClientManager.init()

  second_comm = ClientManager.request('second0')
  received_object = second_comm.recv(source=0)
  
  print('(Recv) 1|Third  <- 0|Second: ', end='')
  print(received_object)

  ClientManager.finalize()

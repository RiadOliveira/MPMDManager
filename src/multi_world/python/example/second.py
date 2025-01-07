from mpi4py import MPI
from abstraction import ServerManager, ClientManager

rank = MPI.COMM_WORLD.Get_rank()

if rank == 0:
  ServerManager.init()
  ServerManager.open()

  third_comm = ServerManager.accept()
  transfer_object = {'name': '0|Second to 1|Third', 'value': 2031}
  third_comm.send(transfer_object, 0)
  
  print('(Send) 0|Second -> 1|Third: ', end=' ')
  print(transfer_object)

  ServerManager.finalize()
if rank == 1:
  ClientManager.init()

  first_comm = ClientManager.connect('first0')
  received_object = first_comm.recv(source=0)
  
  print('(Recv) 1|Second <- 0|First: ', end=' ')
  print(received_object)

  ClientManager.finalize()

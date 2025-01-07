from mpi4py import MPI
from abstraction import ServerManager, ClientManager

rank = MPI.COMM_WORLD.Get_rank()

if rank == 0:
  ServerManager.init()
  ServerManager.open()

  second_comm = ServerManager.accept()
  transfer_object = {'name': '0|First  to 1|Second', 'value': 1021}
  second_comm.send(transfer_object, dest=0)

  print('(Send) 0|First  -> 1|Second: ', end='')
  print(transfer_object)

  ServerManager.finalize()
elif rank == 1:
  ClientManager.init()

  third_comm = ClientManager.connect('third0')
  received_object = third_comm.recv(source=0)
  
  print('(Recv) 1|First  <- 0|Third: ', end=' ')
  print(received_object)

  ClientManager.finalize()

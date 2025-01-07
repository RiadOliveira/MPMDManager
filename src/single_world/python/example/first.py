from abstraction import MPMDManager

MPMDManager.init()
local_rank = MPMDManager.local_rank()

if(local_rank == 0):
  second_comm = MPMDManager.comm_to('second')
  transfer_object = {'text': '0|First  to 1|Second', 'value': 1021}
  second_comm.send(transfer_object, dest=1)
  
  print('(Send) 0|First  -> 1|Second: ', end='')
  print(transfer_object)
elif(local_rank == 1):
  third_comm = MPMDManager.comm_to('third')
  received_object = third_comm.recv(source=0)
  
  print('(Recv) 1|First  <- 0|Third: ', end=' ')
  print(received_object)

MPMDManager.finalize()

from abstraction.mpmd_manager import MPMDManager

MPMDManager.initialize()
local_rank = MPMDManager.local_comm().Get_rank()

if(local_rank == 0):
  transfer_object = {'name': 'First(0) to Second(1)', 'value': 1021}
  MPMDManager.comm_to('second').send(transfer_object, 1)
  
  print('(Send) 0|First  -> 1|Second: ', end='')
  print(transfer_object)
elif(local_rank == 1):
  received_object = MPMDManager.comm_to('third').recv(source=0)
  
  print('(Recv) 1|First  <- 0|Third: ', end=' ')
  print(received_object)

MPMDManager.finalize()

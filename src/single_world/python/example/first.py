from abstraction import MPMDManager

MPMDManager.initialize()
local_rank = MPMDManager.local_comm().Get_rank()

if(local_rank == 0):
  transfer_object = {'name': '0|First  to 1|Second', 'value': 1021}
  MPMDManager.intercomm_to('second').send(transfer_object, 1)
  
  print('(Send) 0|First  -> 1|Second: ', end='')
  print(transfer_object)
elif(local_rank == 1):
  received_object = MPMDManager.intercomm_to('third').recv(source=0)
  
  print('(Recv) 1|First  <- 0|Third: ', end=' ')
  print(received_object)

MPMDManager.finalize()
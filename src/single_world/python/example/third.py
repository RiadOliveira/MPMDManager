from abstraction import MPMDManager

MPMDManager.init()
local_rank = MPMDManager.local_rank()

if(local_rank == 0):
  transfer_object = {'text': '0|Third  to 1|First', 'value': 3011}
  MPMDManager.comm_to('first').send(transfer_object, 1)
  
  print('(Send) 0|Third  -> 1|First: ', end=' ')
  print(transfer_object)
elif(local_rank == 1):
  received_object = MPMDManager.comm_to('second').recv(source=0)
  
  print('(Recv) 1|Third  <- 0|Second: ', end='')
  print(received_object)

MPMDManager.finalize()

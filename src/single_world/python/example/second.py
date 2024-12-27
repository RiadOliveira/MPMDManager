from abstraction import MPMDManager

MPMDManager.init()
local_rank = MPMDManager.local_rank()

if(local_rank == 0):
  transfer_object = {'text': '0|Second to 1|Third', 'value': 2031}
  MPMDManager.comm_to('third').send(transfer_object, 1)
  
  print('(Send) 0|Second -> 1|Third: ', end=' ')
  print(transfer_object)
elif(local_rank == 1):
  received_object = MPMDManager.comm_to('first').recv(source=0)
  
  print('(Recv) 1|Second <- 0|First: ', end=' ')
  print(received_object)

MPMDManager.finalize()

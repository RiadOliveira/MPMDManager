from abstraction import MPMDManager

MPMDManager.init()
local_rank = MPMDManager.local_rank()

if(local_rank == 0):
  third_comm = MPMDManager.comm_to('third')
  transfer_object = {'text': '0|Second to 1|Third', 'value': 2031}
  third_comm.send(transfer_object, dest=1)
  
  print('(Send) 0|Second -> 1|Third: ', end=' ')
  print(transfer_object)
elif(local_rank == 1):
  first_comm = MPMDManager.comm_to('first')
  received_object = first_comm.recv(source=0)
  
  print('(Recv) 1|Second <- 0|First: ', end=' ')
  print(received_object)

MPMDManager.finalize()

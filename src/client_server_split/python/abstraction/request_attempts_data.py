class RequestAttemptsData:
  def __init__(
    self, max_attempts_to_connect = 10,
    initial_attempts_wait_time_in_secs = 0.1,
    max_attempts_wait_time_in_secs = 10
  ):
    self.__max_attempts_to_connect = max_attempts_to_connect
    self.__initial_attempts_wait_time_in_secs = initial_attempts_wait_time_in_secs
    self.__max_attempts_wait_time_in_secs = max_attempts_wait_time_in_secs

  @property
  def max_attempts_to_connect(self): return self.__max_attempts_to_connect

  @property
  def initial_attempts_wait_time_in_secs(self): return self.__initial_attempts_wait_time_in_secs 

  @property
  def max_attempts_wait_time_in_secs(self): return self.__max_attempts_wait_time_in_secs 
    
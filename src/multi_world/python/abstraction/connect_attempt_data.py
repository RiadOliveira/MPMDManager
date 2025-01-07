class ConnectAttemptData:
  def __init__(
    self, max_attempts = 10, initial_wait_ms = 500,
    wait_increment_ms = 500, max_wait_ms = 5000
  ):
    self.__max_attempts = max_attempts
    self.__initial_wait_ms = initial_wait_ms
    self.__wait_increment_ms = wait_increment_ms
    self.__max_wait_ms = max_wait_ms

  @property
  def max_attempts(self): return self.__max_attempts

  @property
  def initial_wait_ms(self): return self.__initial_wait_ms

  @property
  def wait_increment_ms(self): return self.__wait_increment_ms

  @property
  def max_wait_ms(self): return self.__max_wait_ms

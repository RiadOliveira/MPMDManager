class AuxiliarConnection:
  name: str
  leader: int
  size: int

  def __init__(self, name: str, leader: int, size = 1):
    self.name, self.leader, self.size = name, leader, size
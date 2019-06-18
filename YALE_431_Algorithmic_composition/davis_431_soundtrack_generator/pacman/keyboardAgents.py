from game import Agent
from game import Directions
import random
import OSC

c = OSC.OSCClient()
c.connect(('127.0.0.1', 57120))
def sendMessage(msg):
  oscmsg = OSC.OSCMessage()
  oscmsg.setAddress('/sg')
  [oscmsg.append(m) for m in msg]
  c.send(oscmsg)

sendMessage(('global', 'max_voices', '4'))
sendMessage(('global', 'reverb', '0.1'))
sendMessage(('melody', 'play'))
sendMessage(('harmony', 'play'))
sendMessage(('ghostdist1', 'play'))
sendMessage(('ghostdist2', 'play'))

oldNumFood = 0
oldScaredTimes = [0,0]
def sendMessages(state):
  global oldNumFood, oldScaredTimes
  # Useful information you can extract from a GameState (pacman.py)
  pos = state.getPacmanState().getPosition()
  ghostStates = state.getGhostStates() 
  scaredTimes = [ghostState.scaredTimer for ghostState in ghostStates]
  ghostPositions = [g.getPosition() for g in ghostStates]
  numFood = state.getNumFood()

  g = ghostPositions[0]
  dist = ((pos[0] - g[0])**2 + (pos[1] - g[1])**2)**0.5
  sendMessage(('ghostdist1', 'pan', -0.5*(pos[0] - g[0]) / dist))
  sendMessage(('ghostdist1', 'volume', 0.5/dist))
  g = ghostPositions[1]
  dist = ((pos[0] - g[0])**2 + (pos[1] - g[1])**2)**0.5
  sendMessage(('ghostdist2', 'pan', -0.5*(pos[0] - g[0]) / dist))
  sendMessage(('ghostdist2', 'volume', 0.5/dist))


  if oldScaredTimes[0] - 1 > scaredTimes[0] or oldScaredTimes[1] - 1 > scaredTimes[1]:
    sendMessage(('eatghost', 'play'))
  elif numFood < oldNumFood:
    sendMessage(('eatfruit', 'play'))
    sendMessage(('eatfruit', 'volume', 0.5))
  
  if scaredTimes[0] != 0 or scaredTimes[1] != 0:
    sendMessage(('global', 'tempo', '2'))
  else:
    sendMessage(('global', 'tempo', '1'))
  oldNumFood = numFood
  oldScaredTimes = scaredTimes

def onEnd(state):
  if state.isLose():
    sendMessage(('global', 'max_voices', '1'))
    sendMessage(('death', 'play'))
    sendMessage(('melody', 'stop'))
    sendMessage(('harmony', 'stop'))
    sendMessage(('ghostdist1', 'stop'))
    sendMessage(('ghostdist2', 'stop'))


class KeyboardAgent(Agent):
  """
  An agent controlled by the keyboard.
  """
  # NOTE: Arrow keys also work.
  WEST_KEY  = 'a' 
  EAST_KEY  = 'd' 
  NORTH_KEY = 'w' 
  SOUTH_KEY = 's'

  def __init__( self, index = 0 ):
    
    self.lastMove = Directions.STOP
    self.index = index
    self.keys = []
    
  def final(self, state):
    onEnd(state)

  def getAction( self, state):
    sendMessages(state)

    from graphicsUtils import keys_waiting
    from graphicsUtils import keys_pressed
    keys = keys_waiting() + keys_pressed()
    if keys != []:
      self.keys = keys
    
    legal = state.getLegalActions(self.index)
    move = self.getMove(legal)
    
    if move == Directions.STOP:
      # Try to move in the same direction as before
      if self.lastMove in legal:
        move = self.lastMove
        
    if move not in legal:
      move = random.choice(legal)
      
    self.lastMove = move
    return move

  def getMove(self, legal):
    move = Directions.STOP
    if   (self.WEST_KEY in self.keys or 'Left' in self.keys) and Directions.WEST in legal:  move = Directions.WEST
    if   (self.EAST_KEY in self.keys or 'Right' in self.keys) and Directions.EAST in legal: move = Directions.EAST
    if   (self.NORTH_KEY in self.keys or 'Up' in self.keys) and Directions.NORTH in legal:   move = Directions.NORTH
    if   (self.SOUTH_KEY in self.keys or 'Down' in self.keys) and Directions.SOUTH in legal: move = Directions.SOUTH
    return move
  
class KeyboardAgent2(KeyboardAgent):
  """
  A second agent controlled by the keyboard.
  """
  # NOTE: Arrow keys also work.
  WEST_KEY  = 'j' 
  EAST_KEY  = "l" 
  NORTH_KEY = 'i' 
  SOUTH_KEY = 'k'
  
  def getMove(self, legal):
    move = Directions.STOP
    if   (self.WEST_KEY in self.keys) and Directions.WEST in legal:  move = Directions.WEST
    if   (self.EAST_KEY in self.keys) and Directions.EAST in legal: move = Directions.EAST
    if   (self.NORTH_KEY in self.keys) and Directions.NORTH in legal:   move = Directions.NORTH
    if   (self.SOUTH_KEY in self.keys) and Directions.SOUTH in legal: move = Directions.SOUTH
    return move
  
  

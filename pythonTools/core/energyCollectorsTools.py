from abc import ABC, abstractmethod
from os import read
import matplotlib.pyplot as plt

#python tools for visualization of energy collectors

class Vec3:
  def __init__(self, x, y, z):  
    self.x = float(x)
    self.y = float(y)
    self.z = float(z)

class ObjectInterface(ABC): 
  @abstractmethod
  def plotXSideView(self):
    return 
  @abstractmethod
  def plotYSideView(self):
    return

class Ray(ObjectInterface):
  def __init__(self, origin, direction):
    assert isinstance(origin, Vec3)
    self.origin = origin
    assert isinstance(direction, Vec3)
    self.direction = direction

  def plotXSideView(self):
    xTemp = [self.origin.x, self.origin.x + self.direction.x]
    yTemp = [self.origin.z, self.origin.z + self.direction.z]
    plt.plot(xTemp, yTemp)

  def plotYSideView(self):
    xTemp = [self.origin.y, self.origin.y + self.direction.y]
    yTemp = [self.origin.z, self.origin.z + self.direction.z]
    plt.plot(xTemp, yTemp)

class EnergyCollector(ObjectInterface):
  def __init__(self, origin, radius):
    assert isinstance(origin, Vec3)
    self.origin = origin
    self.radius = float(radius)

  def plotXSideView(self):
    position = (self.origin.x, self.origin.z)
    
    collectorCircle = plt.Circle(position, radius=self.radius)
    collectorCircle.set_fill(False)
    collectorCircle.set_color('blue')
    plt.gca().add_patch(collectorCircle)

    collectorOrigin = plt.Circle(position, radius = self.radius / 20)
    collectorOrigin.set_fill(True)
    collectorOrigin.set_color('red')
    plt.gca().add_patch(collectorOrigin)

  def plotYSideView(self):
    position = (self.origin.y, self.origin.z)
    
    collectorCircle = plt.Circle(position, radius=self.radius)
    collectorCircle.set_fill(False)
    collectorCircle.set_color('blue')
    plt.gca().add_patch(collectorCircle)

    collectorOrigin = plt.Circle(position, radius = self.radius / 20)
    collectorOrigin.set_fill(True)
    collectorOrigin.set_color('red')
    plt.gca().add_patch(collectorOrigin)

class SphereWall(ObjectInterface):
  def __init__(self, origin, radius):
    assert isinstance(origin, Vec3)
    self.origin = origin
    self.radius = float(radius)

  def plotXSideView(self):
    position = (self.origin.x, self.origin.z)
    SimulationCircle = plt.Circle(position, radius=self.radius)
    SimulationCircle.set_fill(False)
    plt.gca().add_patch(SimulationCircle)

  def plotYSideView(self):
    position = (self.origin.y, self.origin.z)
    SimulationCircle = plt.Circle(position, radius=self.radius)
    SimulationCircle.set_fill(False)
    plt.gca().add_patch(SimulationCircle)

class CollectorReader:
  def __init__(self, filePath):
    self.path = filePath
    self.objList = []
  
  def getReadCollector(self, collectorString):
    firstSlice = collectorString[collectorString.find("Vec3(")+5:]
    x = firstSlice[:firstSlice.find(',')]
    secondSlice = firstSlice[firstSlice.find(',')+2:]
    y = secondSlice[:secondSlice.find(',')]
    thirdSlice = secondSlice[secondSlice.find(',')+2:]
    z = thirdSlice[:thirdSlice.find(')')]
    radius = thirdSlice[thirdSlice.find("Radius: ")+ 8: thirdSlice.find("\n")]
    return EnergyCollector(Vec3(x, y, z), radius)

  def readFile(self):
    with open(self.path) as f:
      for line in f.readlines():
        self.objList.append(self.getReadCollector(line))
    return self.objList
  
  def simulationRadius(self):
    maximum = 0
    for collector in self.objList:
      maximum = max(maximum, collector.origin.z)  
    return maximum  

class Plotter:
  def __init__(self, filePath):
    reader = CollectorReader(filePath)
    self.objList = reader.readFile()
    self.simRadius = reader.simulationRadius()
    self.objList.append(SphereWall(Vec3(0, 0, 0), self.simRadius))
  
  def plotXView(self):
    for obj in self.objList:
      obj.plotXSideView()
    plt.title("X Side view")
    lim = 1.5 * self.simRadius
    plt.ylim(0, lim)
    plt.xlim(-lim, lim)
  
  def plotYView(self):
    for obj in self.objList:
      obj.plotYSideView()
    plt.title("Y Side view")
    lim = 1.5 * self.simRadius
    plt.ylim(0, lim)
    plt.xlim(-lim, lim)

  def plot(self):
    plt.figure()
    plt.subplot(2,1,1)
    self.plotXView()
    plt.subplot(2,1,2)
    self.plotYView()
    plt.show()
    
    

           

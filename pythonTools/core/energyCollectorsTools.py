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
  @abstractmethod
  def plotXYView(self):
    return 

class Section(ObjectInterface):
  def __init__(self, point1, point2):
    assert isinstance(point1, Vec3)
    self.point1 = point1
    assert isinstance(point2, Vec3)
    self.point2 = point2

  def plotXSideView(self):
    xTemp = [self.point1.x, self.point2.x]
    yTemp = [self.point1.z, self.point2.z]
    plt.plot(xTemp, yTemp)

  def plotYSideView(self):
    xTemp = [self.point1.y, self.point2.y]
    yTemp = [self.point1.z, self.point2.z]
    plt.plot(xTemp, yTemp)
  
  def plotXYView(self):
    xTemp = [self.point1.x, self.point2.x]
    yTemp = [self.point1.y, self.point2.y]
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
  
  def plotXYView(self):
    position = (self.origin.x, self.origin.y)
    
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

  def plotXYView(self):
    position = (self.origin.x, self.origin.y)
    SimulationCircle = plt.Circle(position, radius=self.radius)
    SimulationCircle.set_fill(False)
    plt.gca().add_patch(SimulationCircle)

class CollectorReader:
  def __init__(self, collectorFilePath, sectionFilePath=None):
    self.pathCollectors = collectorFilePath
    self.pathSections = sectionFilePath
    self.objList = []
    self.sectionList = []
  
  def getReadCollector(self, collectorString):
    firstSlice = collectorString[collectorString.find("Vec3(")+5:]
    x = firstSlice[:firstSlice.find(',')]
    secondSlice = firstSlice[firstSlice.find(',')+2:]
    y = secondSlice[:secondSlice.find(',')]
    thirdSlice = secondSlice[secondSlice.find(',')+2:]
    z = thirdSlice[:thirdSlice.find(')')]
    radius = thirdSlice[thirdSlice.find("Radius: ")+ 8: thirdSlice.find("\n")]
    return EnergyCollector(Vec3(x, y, z), radius)

  def getReadSection(self, sectionString):
    firstSlice = sectionString[sectionString.find("Vec3(")+5:sectionString.find(" to: ")]
    x1 = firstSlice[:firstSlice.find(',')]
    secondSlice = firstSlice[firstSlice.find(',')+2:]
    y1 = secondSlice[:secondSlice.find(',')]
    thirdSlice = secondSlice[secondSlice.find(',')+2:]
    z1 = thirdSlice[:thirdSlice.find(')')]

    firstSlice = sectionString[sectionString.find("to"):]
    firstSlice = firstSlice[firstSlice.find("Vec3(")+5:]
    x2 = firstSlice[:firstSlice.find(',')]
    secondSlice = firstSlice[firstSlice.find(',')+2:]
    y2 = secondSlice[:secondSlice.find(',')]
    thirdSlice = secondSlice[secondSlice.find(',')+2:]
    z2 = thirdSlice[:thirdSlice.find(')')]
    
    return Section(Vec3(x1, y1, z1), Vec3(x2, y2, z2))

  def readCollectors(self):
    with open(self.pathCollectors) as f:
      for line in f.readlines():
        self.objList.append(self.getReadCollector(line))
    return self.objList

  def readSection(self):
    if (self.pathSections is not None):
      with open(self.pathSections)  as f:
        for line in f.readlines():
          self.sectionList.append(self.getReadSection(line))
      return self.sectionList
  
  def simulationRadius(self):
    maximum = 0
    for collector in self.objList:
      maximum = max(maximum, collector.origin.z)  
    return maximum  

class Plotter:
  def __init__(self, collectorPath, sectionPath= None):
    reader = CollectorReader(collectorPath, sectionPath)
    self.collectorPath = collectorPath
    self.sectionPath = sectionPath 
    self.objList = reader.readCollectors()
    
    if (self.sectionPath is not None):
      self.sectionList = reader.readSection()
    else:
      self.sectionList = []

    self.simRadius = reader.simulationRadius()
    self.objList.append(SphereWall(Vec3(0, 0, 0), self.simRadius))
  
  def plotXView(self):
    for obj in self.objList:
      obj.plotXSideView()

    if self.sectionPath is not None:
      for section in self.sectionList:
        section.plotXSideView()
    
    plt.title("XZ Side view")
    lim = 1.5 * self.simRadius
    plt.ylim(0, lim)
    plt.xlim(-lim, lim)
  
  def plotYView(self):
    for obj in self.objList:
      obj.plotYSideView()
    
    if self.sectionPath is not None:
      for section in self.sectionList:
        section.plotYSideView()

    plt.title("YZ Side view")
    lim = 1.5 * self.simRadius
    plt.ylim(0, lim)
    plt.xlim(-lim, lim)

  def plotXYView(self):
    for obj in self.objList:
      obj.plotXYView()
    
    if self.sectionPath is not None:
      for section in self.sectionList:
        section.plotXYView()

    plt.title("XY Side view")
    lim = 1.5 * self.simRadius
    plt.ylim(-lim, lim)
    plt.xlim(-lim, lim)

  def save(self, path, plot=False):
    plt.figure(figsize=(9,9))
    
    plt.subplot(2,2,2)
    self.plotXView()
    plt.subplot(2,2,3)
    self.plotYView()

    plt.subplot(2,2,1)
    self.plotXYView()

    plt.savefig(path)
    if plot:
      plt.show()
    
    

           

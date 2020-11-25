from abc import ABC, abstractmethod
from os import read
import matplotlib.pyplot as plt
import random
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
    self.color = []
    for _ in range(3):
      self.color.append(random.random())

  def plotXSideView(self):
    xTemp = [self.point1.x, self.point2.x]
    yTemp = [self.point1.z, self.point2.z]
    plt.plot(xTemp, yTemp, color=self.color)

  def plotYSideView(self):
    xTemp = [self.point1.y, self.point2.y]
    yTemp = [self.point1.z, self.point2.z]
    plt.plot(xTemp, yTemp,color=self.color)
  
  def plotXYView(self):
    xTemp = [self.point1.x, self.point2.x]
    yTemp = [self.point1.y, self.point2.y]
    plt.plot(xTemp, yTemp, color=self.color)

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

class Triangle(ObjectInterface):
  def __init__(self, point1, point2, point3):
    self.point1 = point1
    self.point2 = point2
    self.point3 = point3
  
  def plotXSideView(self):
    triangle = plt.Polygon([[self.point1.x, self.point1.z],
       [self.point2.x, self.point2.z], [self.point3.x, self.point3.z]])
    triangle.set_edgecolor("black")
    color = []
    for _ in range(3):
      color.append(random.random())
    triangle.set_fc(color)
    plt.gca().add_patch((triangle))

  def plotYSideView(self):
    triangle = plt.Polygon([[self.point1.y, self.point1.z],
        [self.point2.y, self.point2.z], [self.point3.y, self.point3.z]])
    plt.gca().add_patch((triangle))

  def plotXYView(self):
    triangle = plt.Polygon([[self.point1.x, self.point1.y],
        [self.point2.x, self.point2.y], [self.point3.x, self.point3.y]])
    plt.gca().add_patch((triangle))

class Reader:
  def __init__(self, modelFilePath=None ,collectorFilePath=None, sectionFilePath=None):
    self.modelFilePath = modelFilePath
    self.pathCollectors = collectorFilePath
    self.pathSections = sectionFilePath
    self.collectorList = []
    self.sectionList = []
    self.triangles = []
  
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

  def getReadTriangle(self, triangleString):

    firstSlice = triangleString[triangleString.find("Vec3(")+5:]
    x1 = firstSlice[:firstSlice.find(',')]
    secondSlice = firstSlice[firstSlice.find(',')+2:]
    y1 = secondSlice[:secondSlice.find(',')]
    thirdSlice = secondSlice[secondSlice.find(',')+2:]
    z1 = thirdSlice[:thirdSlice.find(')')]

    firstSlice = thirdSlice[thirdSlice.find("Vec3(")+5:]
    x2 = firstSlice[:firstSlice.find(',')]
    secondSlice = firstSlice[firstSlice.find(',')+2:]
    y2 = secondSlice[:secondSlice.find(',')]
    thirdSlice = secondSlice[secondSlice.find(',')+2:]
    z2 = thirdSlice[:thirdSlice.find(')')]

    firstSlice = thirdSlice[thirdSlice.find("Vec3(")+5:]
    x3 = firstSlice[:firstSlice.find(',')]
    secondSlice = firstSlice[firstSlice.find(',')+2:]
    y3 = secondSlice[:secondSlice.find(',')]
    thirdSlice = secondSlice[secondSlice.find(',')+2:]
    z3 = thirdSlice[:thirdSlice.find(')')]

    return Triangle(Vec3(x1, y1, z1), Vec3(x2, y2,z2), Vec3(x3, y3, z3))

  def readCollectors(self):
    with open(self.pathCollectors) as f:
      for line in f.readlines():
        self.collectorList.append(self.getReadCollector(line))
    return self.collectorList

  def readSection(self):
    if (self.pathSections is not None):
      with open(self.pathSections)  as f:
        for line in f.readlines():
          self.sectionList.append(self.getReadSection(line))
      return self.sectionList

  def readTriangles(self):
    if (self.modelFilePath is not None):
      with open(self.modelFilePath) as f:
        for line in f.readlines():
          self.triangles.append(self.getReadTriangle(line))
    return self.triangles

  def simulationRadius(self):
    maximum = 0
    for collector in self.collectorList:
      maximum = max(maximum, collector.origin.z)  
    return maximum  

class Plotter:
  def __init__(self, modelPath=None, collectorPath=None, sectionPath=None):
    reader = Reader(modelPath ,collectorPath, sectionPath)
    self.collectorPath = collectorPath
    self.sectionPath = sectionPath 
    self.modelPath = modelPath

    if self.collectorPath is not None:
      self.collectorList = reader.readCollectors()
      self.simRadius = reader.simulationRadius()
    else:
      self.simRadius = 4
      self.collectorList = []

    if (self.sectionPath is not None):
      self.sectionList = reader.readSection()
    else:
      self.sectionList = []

    if self.modelPath is not None:
      self.triangleList = reader.readTriangles()
    else:
      self.triangleList = []

    self.collectorList.append(SphereWall(Vec3(0, 0, 0), self.simRadius))
  
  def plotXView(self):
    if self.collectorPath is not None:
      for collector in self.collectorList:
        collector.plotXSideView()

    if self.modelPath is not None:
      for triangle in self.triangleList:
        triangle.plotXSideView()

    if (self.sectionPath is not None):
      for section in self.sectionList:
        section.plotXSideView()
    
    plt.title("XZ Side view")
    lim = 1.5 * self.simRadius
    plt.ylim(0, lim)
    plt.xlim(-lim, lim)
  
  def plotYView(self):
    if self.collectorPath is not None:
      for collector in self.collectorList:
        collector.plotYSideView()
    
    if self.sectionPath is not None:
      for section in self.sectionList:
        section.plotYSideView()

    if self.modelPath is not None:
      for triangle in self.triangleList:
        triangle.plotYSideView()

    plt.title("YZ Side view")
    lim = 1.5 * self.simRadius
    plt.ylim(0, lim)
    plt.xlim(-lim, lim)

  def plotXYView(self):
    if self.collectorPath is not None:
      for collector in self.collectorList:
        collector.plotXYView()
      
    if self.sectionPath is not None:
      for section in self.sectionList:
        section.plotXYView()

    if self.modelPath is not None:
      for triangle in self.triangleList:
        triangle.plotXYView()

    plt.title("XY Side view")
    lim = 1.5 * self.simRadius
    plt.ylim(-lim, lim)
    plt.xlim(-lim, lim)

  def save(self, path, plot=False):
    plt.figure(figsize=(9,9))
    
    plt.subplot(2,2,3)
    self.plotXView()
    plt.subplot(2,2,2)
    self.plotYView()

    plt.subplot(2,2,1)
    self.plotXYView()

    plt.savefig(path)
    if plot:
      plt.show()


    
    

           

import pythonTools.core.energyCollectorsTools as tools

reader = tools.CollectorReader(collectorFilePath="skip", sectionFilePath="skip")
collector = reader.getReadCollector(collectorString="5 Energy Collector. Origin: Vec3(3.93923, 0, 0.694593), Radius: 0.697246\n")
section = reader.getReadSection(sectionString = "from: Vec3(0.333333, -5, 0.333333) to: Vec3(0.333333, 0, 0.333333)")

def test_BuildCollector():
  assert collector.origin.x == 3.93923
  assert collector.origin.y == 0
  assert collector.origin.z == 0.694593
  assert collector.radius == 0.697246

def test_Section():
  assert section.point1.x == 0.333333
  assert section.point1.y == -5
  assert section.point1.z == 0.333333

  assert section.point2.x == 0.333333
  assert section.point2.y == 0
  assert section.point2.z == 0.333333
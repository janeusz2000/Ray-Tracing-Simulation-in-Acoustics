import pythonTools.core.energyCollectorsTools as tools

reader = tools.CollectorReader(filePath="skip")
collector = reader.getReadCollector(collectorString="5 Energy Collector. Origin: Vec3(3.93923, 0, 0.694593), Radius: 0.697246\n")

def test_BuildCollector():
  assert collector.origin.x == 3.93923
  assert collector.origin.y == 0
  assert collector.origin.z == 0.694593
  assert collector.radius == 0.697246

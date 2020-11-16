import pythonTools.energyCollectorsTools as tools

reader = tools.CollectorReader(filePath="skip")
collector = reader.getReadCollector(collectorString="0 Energy Collector. Origin: Vec3(-4.234, 0.2345, 0.2356), Radius: 0.738147\n")

def test_BuildCollector():
  assert collector.origin.x == -4.234
  assert collector.origin.y == 0.2345
  assert collector.origin.z == 0.2356
  assert collector.radius == 0.738147

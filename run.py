import pythonTools.core.energyCollectorsTools as tools

def run():
  plotter = tools.Plotter(filePath="pythonTools/energyCollectors.txt")
  plotter.plotXView()
  plotter.plotYView()

if __name__ == "__main__":
  run()
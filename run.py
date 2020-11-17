import pythonTools.core.energyCollectorsTools as tools

def run():
  plotter = tools.Plotter(filePath="D:\\cpp_projects\\magisterkaCPP\\pythonTools\\energyCollectors.txt")
  plotter.plot()

if __name__ == "__main__":
  run()
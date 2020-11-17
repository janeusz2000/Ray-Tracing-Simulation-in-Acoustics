import pythonTools.core.energyCollectorsTools as tools

# creates picture of energyCollectors from given
# txt file 
def run():
  plotter = tools.Plotter(filePath="D:\\cpp_projects\\magisterkaCPP\\pythonTools\\data\\energyCollectors.txt")
  plotter.save(plot=False)

if __name__ == "__main__":
  run()
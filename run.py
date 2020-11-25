import pythonTools.core.energyCollectorsTools as tools

# creates picture of energyCollectors from energyCollectors.txt
# txt file and draws all hits from sectionData.txt
def run():
  plotter = tools.Plotter(
    collectorPath="D:\\cpp_projects\\magisterkaCPP\\pythonTools\\data\\energyCollectors.txt",
    sectionPath="D:\\cpp_projects\\magisterkaCPP\\pythonTools\\data\\sectionsData.txt" )
  plotter.save("D:\\cpp_projects\\magisterkaCPP\\pythonTools\\data\\test" ,plot=True)

if __name__ == "__main__":
  run()
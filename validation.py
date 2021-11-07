from dataclasses import dataclass
import json
import logging
import os
import pprint

import validationTools.compareResultsToReference as compTool
from validationTools.logValidation import performLogging
from validationTools.loggingSimulationProperties import logSimulationProperties

def setUpLogger() -> logging.Logger:
    logger = logging.getLogger('Validaton Logger')
    logger.setLevel(logging.DEBUG)

    channel = logging.StreamHandler()
    channel.setLevel(logging.DEBUG)

    formatter = logging.Formatter(
        '%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    channel.setFormatter(formatter)
    logger.addHandler(channel)
    return logger

logger = setUpLogger()

@dataclass
class DiffusorProperty:
    name: str
    path: str
    type: str


@dataclass
class SimulationProperties:
    sourcePower: float
    numOfCollectors: int
    numOfRaysSquared: int
    maxTracking: int

def executeCommand(command: str) -> None:
    os.system(command)


def buildBinaries():
    os.system("bazel build --config=_gcc validationXAxis")
    os.system("bazel build --config=_gcc validationBothAxis")
    os.system("bazel build --config=_gcc validationGeoDome")


def executeLineValidation(diffusorProperty: DiffusorProperty,
                          simulationProperties: SimulationProperties,
                          raportPath: str) -> None:
    command = ""
    if diffusorProperty.type == "xAxis":
        command = f"bazel-bin/validationXAxis  {raportPath} {diffusorProperty.path} {simulationProperties.sourcePower} {simulationProperties.numOfCollectors} {simulationProperties.numOfRaysSquared} {simulationProperties.maxTracking}"
    else:
        command = f"bazel-bin/validationBothAxis  {raportPath} {diffusorProperty.path} {simulationProperties.sourcePower} {simulationProperties.numOfCollectors} {simulationProperties.numOfRaysSquared} {simulationProperties.maxTracking}"
    executeCommand(command)


def executeSurfaceValidation(diffusorProperty: DiffusorProperty,
                             simulationProperties: SimulationProperties,
                             raportPath: str) -> None:
    command = f"bazel-bin/validationGeoDome {raportPath} {diffusorProperty.path} {simulationProperties.sourcePower} {simulationProperties.numOfCollectors} {simulationProperties.numOfRaysSquared} {simulationProperties.maxTracking}"
    executeCommand(command)


def getDiffusorPropertyList() -> list:
    diffusorPropertiesList = list()
    for dirpath, dnames, fNames in os.walk("./validationDiffusors"):
        for f in fNames:
            if f.endswith(".obj"):
                diffusorType = "xAxis" if "1D" in f else "xyAxis"
                diffusor = DiffusorProperty(
                    f[:-4], os.path.join(dirpath, f), diffusorType)
                diffusorPropertiesList.append(diffusor)
    return diffusorPropertiesList


def createLineRaportFile(name: str) -> str:
    path = f"./ValidationResults/rayTraced/line/{name}.json"
    executeCommand("rm " + path)
    executeCommand("touch " + path)
    return path


def createSurfaceRaportFile(name: str) -> str:
    path = f"./ValidationResults/rayTraced/surface/{name}.json"
    print(f"Created raport file at: {path}")
    executeCommand("rm " + path)
    executeCommand("touch " + path)
    return path

def getAnalyticalReferencePath(name: str) -> str:
    return f"./ValidationResults/Reference/analitical/{name}.json"

def getLineReferencePath(name: str) -> str:
    return f"./validationResults/Reference/line/{name}.json"

def getSurfaceReferencePath(name: str) -> str:
    return f"./validationResults/Reference/surface/{name}.json"

def executeLineValidationWithAnalyticalTools(
        lineRaportPath: str,
        name: str):
    analyticalPath = getAnalyticalReferencePath(name=name)
    compTool.executeComparisonAndSaveToDatabase(analyticalPath, lineRaportPath, analytical=True)
    

def executeValidation(simulationProperties, description: str):

    buildBinaries()
    logger.info("Starting validation")
    logSimulationProperties(simulationProperties=simulationProperties)

    logger.info(f"Configuration:")
    logger.info(f"\tSource Power {simulationProperties.sourcePower} [W]")
    logger.info(f"\tNum Of collectors: {simulationProperties.numOfCollectors}")
    logger.info(f"\tNum of rays: {simulationProperties.numOfRaysSquared**2}")
    logger.info(f"\tMax tracking: {simulationProperties.maxTracking}")

    DiffusorPropertyList = getDiffusorPropertyList()
    for diffusor in DiffusorPropertyList:
        lineRaportPath = createLineRaportFile(diffusor.name)
        executeLineValidation(diffusorProperty=diffusor,
                              simulationProperties=simulationProperties,
                              raportPath=lineRaportPath)
        lineReferencePath = getLineReferencePath(diffusor.name)
        compTool.executeComparisonAndSaveToDatabase(
            lineReferencePath, lineRaportPath)
        
        if "1D" in diffusor.name:
            executeLineValidationWithAnalyticalTools(
                    lineRaportPath,
                    name=diffusor.name)

        surfaceRaportPath = createSurfaceRaportFile(diffusor.name)
        executeSurfaceValidation(diffusorProperty=diffusor,
                                 simulationProperties=simulationProperties,
                                 raportPath=surfaceRaportPath)
        surfaceReferencePath = getSurfaceReferencePath(diffusor.name)
        compTool.executeComparisonAndSaveToDatabase(
            surfaceReferencePath, surfaceRaportPath
        )

    performLogging(description)
    logger.info("Validation finished!")


if __name__ == "__main__":

    # simulationPropertiesList = [SimulationProperties(sourcePower=1000, numOfCollectors=33,
    #                          numOfRaysSquared=20, maxTracking=2)]
    # for simulationProperties in simulationPropertiesList:
    #     executeValidation(simulationProperties, "numOfCollectors2")

    # simulationPropertiesList = [SimulationProperties(sourcePower=1000, numOfCollectors=33, numOfRaysSquared=20, maxTracking=4),
    #                             SimulationProperties(
    #                                 sourcePower=1000, numOfCollectors=33, numOfRaysSquared=40, maxTracking=4),
    #                             SimulationProperties(
    #                                 sourcePower=1000, numOfCollectors=33, numOfRaysSquared=60, maxTracking=4),
    #                             SimulationProperties(
    #                                 sourcePower=1000, numOfCollectors=33, numOfRaysSquared=80, maxTracking=4),
    #                             SimulationProperties(sourcePower=1000, numOfCollectors=33, numOfRaysSquared=100, maxTracking=4)]

    # for simulationProperties in simulationPropertiesList:
    #     executeValidation(simulationProperties, "numOfRays")

    # simulationPropertiesList = [SimulationProperties(
    #     sourcePower=1000, numOfCollectors=33, numOfRaysSquared=20, maxTracking=2), SimulationProperties(
    #     sourcePower=1000, numOfCollectors=33, numOfRaysSquared=20, maxTracking=3), SimulationProperties(
    #     sourcePower=1000, numOfCollectors=33, numOfRaysSquared=20, maxTracking=4), SimulationProperties(
    #     sourcePower=1000, numOfCollectors=33, numOfRaysSquared=20, maxTracking=5)]

    # for simulationProperties in simulationPropertiesList:
    #     executeValidation(simulationProperties, "maxTracking")

    simulationPropertiesList = [
        SimulationProperties(sourcePower=1000, numOfCollectors=21,
                             numOfRaysSquared=20, maxTracking=3),
        SimulationProperties(sourcePower=1000, numOfCollectors=25,
                             numOfRaysSquared=20, maxTracking=3),
        SimulationProperties(sourcePower=1000, numOfCollectors=29,
                             numOfRaysSquared=20, maxTracking=3),
        SimulationProperties(sourcePower=1000, numOfCollectors=33,
                             numOfRaysSquared=20, maxTracking=3),
        SimulationProperties(sourcePower=1000, numOfCollectors=37,
                             numOfRaysSquared=20, maxTracking=3)
    ]
    for simulationProperties in simulationPropertiesList:
        executeValidation(simulationProperties, "numOfCollectors2")

    # for simulationProperties in simulationPropertiesList:
    #     executeValidation(simulationProperties, "numOfCollectors")



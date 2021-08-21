import json
import os


def readJson(file):
    with open(file, encoding='utf-8') as f:
        return json.loads(f.read())


def parseSizeFromPath(path) -> str:
    if "surface" in path:
        number = path[path.find("surface") + len("surface"):path.rfind("m")]
    elif "line" in path:
        number = path[path.find("line") + len("line"):path.rfind("m")]
    return number[number.find("_") + 1:number.rfind("m") + 1]


def findKeyInPaths(key, listOfPath) -> str:
    for path in listOfPath:
        if key in path:
            return path
    return None


if __name__ == "__main__":

    projectPath = os.getcwd()
    validationPath = os.path.join(projectPath, "validationResults")

    referencePath = os.path.join(validationPath, "Reference")
    referencePlatePath = os.path.join(referencePath, "referencePlate")

    lineReferencePlatePath = os.path.join(referencePlatePath, "line")
    surfaceReferencePlatePath = os.path.join(referencePlatePath, "surface")

    lineReferencePlates = [os.path.join(lineReferencePlatePath, file) for file in os.listdir(
        lineReferencePlatePath) if (os.path.isfile(os.path.join(lineReferencePlatePath, file)))]

    surfaceReferencePlates = [os.path.join(surfaceReferencePlatePath, file) for file in os.listdir(
        surfaceReferencePlatePath) if (os.path.isfile(os.path.join(surfaceReferencePlatePath, file)))]

    lineReferencePath = os.path.join(referencePath, "line")
    surfaceReferencePath = os.path.join(referencePath, "surface")

    lineReferenceSamples = [os.path.join(lineReferencePath, file) for file in os.listdir(
        lineReferencePath) if (os.path.isfile(os.path.join(lineReferencePath, file)))]

    surfaceReferenceSamples = [os.path.join(surfaceReferencePath, file) for file in os.listdir(
        lineReferencePath) if (os.path.isfile(os.path.join(surfaceReferencePath, file)))]

    for path in lineReferenceSamples:
        sizeKeyword = parseSizeFromPath(path)
        print(
            f"path: {path} : {findKeyInPaths(sizeKeyword, lineReferencePlates)}")
        actualData = readJson(path)
        referencePlate = readJson(findKeyInPaths(
            sizeKeyword, lineReferencePlates))

        actualFrequencies = actualData[0]["frequencies"]
        referenceFrequencies = referencePlate[0]["frequencies"]

        assert actualFrequencies == referenceFrequencies

        parameterValues = list()
        for index in range(len(actualFrequencies)):
            parameterValues.append(max((actualData[0]["values"][index] - referencePlate[0]["values"]
                                        [index]) / (1 - referencePlate[0]["values"][index]), 0))

        newJson = {"name": "Normalized Acoustic Diffusion Coefficient",
                   "values": parameterValues, "frequencies": actualFrequencies}
        actualData.append(newJson)
        with open(path, 'w+') as f:
            f.write(json.dumps(actualData))

    for path in surfaceReferenceSamples:
        sizeKeyword = parseSizeFromPath(path)
        print(
            f"path: {path} : {findKeyInPaths(sizeKeyword, lineReferencePlates)}")
        actualData = readJson(path)
        referencePlate = readJson(findKeyInPaths(
            sizeKeyword, lineReferencePlates))

        actualFrequencies = actualData[0]["frequencies"]
        referenceFrequencies = referencePlate[0]["frequencies"]

        assert actualFrequencies == referenceFrequencies

        parameterValues = list()
        for index in range(len(actualFrequencies)):
            parameterValues.append(max((actualData[0]["values"][index] - referencePlate[0]["values"]
                                        [index]) / (1 - referencePlate[0]["values"][index]), 0))

        newJson = {"name": "Normalized Acoustic Diffusion Coefficient",
                   "values": parameterValues, "frequencies": actualFrequencies}
        actualData.append(newJson)
        with open(path, 'w+') as f:
            f.write(json.dumps(actualData))
        # print("===================")

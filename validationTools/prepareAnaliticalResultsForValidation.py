import json
import os

PATH = os.getcwd()

def executeCommnad(shellCommand: str):
    os.system(shellCommand)

def readJson(file):
    with open(file, encoding='utf-8') as f:
        return json.loads(f.read())

if __name__ == "__main__":
    validatonResultsPath = os.path.join(PATH, "ValidationResults")
    referenceValidationPath = os.path.join(validatonResultsPath, "Reference")

    # this one has two json with results that i need to overwrite with normalized diffusion coefficient
    analyticalPath = os.path.join(referenceValidationPath, "analitical")

    referencePlatePath = os.path.join(referenceValidationPath, "referencePlate")
    analyticalReferencePlatePath= os.path.join(referencePlatePath, "analitical")

    diffusorsResults = list()
    for filename in os.listdir(analyticalPath):
        filePath = os.path.join(analyticalPath, filename)
        diffusorsResults.append(filePath)

    referencePlates = list()
    for filename in os.listdir(analyticalReferencePlatePath):
        filePath = os.path.join(analyticalReferencePlatePath, filename)
        referencePlates.append(filePath)

    for pathIndex in range(len(diffusorsResults)):
        diffusor = readJson(diffusorsResults[pathIndex])
        reference = readJson(referencePlates[pathIndex])

        outputValues = list()
        for index in range(len(diffusor[0]["values"])):
            actual = diffusor[0]["values"][index]
            ref = reference[0]["values"][index]
            value = (actual - ref) / (1 - ref)
            outputValues.append(value if value > 0 else 0)
        
        diffusor.append({"values" : outputValues,
            "name" : "Normalized Acoustic Diffusion Coefficient",
            "frequencies" : diffusor[0]["frequencies"]})

        with open(diffusorsResults[pathIndex], "w") as f:
            f.writelines(str(diffusor))

import json
import numpy as np
from validationTools.loggingStatistics import insert_statistic_Values

# flags should be:
# 1. path to json file that contains reference result
# 2. path to json file that contains acquired results invalidation process


def ElementsAreArray(array1, array2):
    if len(array1) != len(array2):
        return False
    for index in range(len(array1)):
        if (array1[index] != array2[index]):
            return False
    return True


def prepareParamterMap(parameterMap):
    outputMap = {}
    for parameterIndex in range(len(parameterMap)):
        parameterName = parameterMap[parameterIndex]["name"]
        outputMap[parameterName] = {
            "values": parameterMap[parameterIndex]["values"],
            "frequencies": parameterMap[parameterIndex]["frequencies"]}
    return outputMap


def meanErrorDifference(array1, array2) -> float:
    result = np.array(array1)
    reference = np.array(array2)
    return np.sum(np.abs(result - reference)) / reference.size


def standardDeviationError(array1, array2):
    result = np.array(array1)
    reference = np.array(array2)
    return np.std(np.abs(result - reference))


def RMSE(array1, array2) -> float:
    result = np.array(array1)
    reference = np.array(array2)
    return np.square(np.sum(np.power(result - reference, 2)) / result.size)


def maxError(array1, array2) -> float:
    result = np.array(array1)
    reference = np.array(array2)
    return np.max(np.abs(result - reference)) / result.size


def medianError(array1, array2) -> float:
    result = np.array(array1)
    reference = np.array(array2)
    return np.median(np.abs(result - reference))


def minError(array1, array2) -> float:
    result = np.array(array1)
    reference = np.array(array2)
    return np.min(np.abs(result - reference)) / result.size


def extractNameFromPath(path: str):
    modelDictionary = {"1D_1m_modulo23_500Hz_46n_30stopni_5potega.json": "Model A",
                       "1D_2m_modulo13_250Hz_9n_15stopni_5potega.json": "Model B",
                       "2D_1m_200Hz_modulo7_30stopni_6n.json": "Model D",
                       "2D_2m_6n_modulo7_200Hz_15stopni_5potega.json": "Model C"}
    simulationType = "line" if "line" in path else "surface"
    return f"{simulationType} {modelDictionary[path[path.rfind('/')+1:]]}"


def executeComparisonAndSaveToDatabase(referencePath: str, resultsPath: str):

    with open(referencePath) as jsonFile:
        reference = json.load(jsonFile)
    with open(resultsPath) as jsonFile:
        results = json.load(jsonFile)

    resultMap = prepareParamterMap(results)
    referenceMap = prepareParamterMap(reference)

    for parameter_name in referenceMap.keys():
        result = resultMap[parameter_name]
        reference = referenceMap[parameter_name]

        # CHECK FREQUENCIES
        areFrequenciesTheSame = ElementsAreArray(
            result["frequencies"], reference["frequencies"])

        if not areFrequenciesTheSame:
            errorMessage = f"""Frequencies of results {result['frequencies']} \
                must be the same as frequencies of the \
                    reference {reference['frequencies']}"""
            raise ValueError(errorMessage)

        else:
            parameters = [meanErrorDifference,
                          standardDeviationError,
                          RMSE,
                          maxError,
                          minError,
                          medianError]

            outputMessage = " Validation values: \n"
            outputValues = list()
            for parameter in parameters:
                name = str(parameter.__name__)
                float_value = round(
                    parameter(result['values'], reference['values']), 5)
                value = str(float_value)
                outputMessage += f"\t{name} : {value}\n"
                outputValues.append(float_value)
            insert_statistic_Values(
                parameterName=parameter_name,
                modelName=extractNameFromPath(resultsPath),
                meanError=outputValues[0],
                standardDeviation=outputValues[1],
                rmse=outputValues[2],
                maxError=outputValues[3],
                minError=outputValues[4],
                medianError=outputValues[5])

    return len(referenceMap.keys())

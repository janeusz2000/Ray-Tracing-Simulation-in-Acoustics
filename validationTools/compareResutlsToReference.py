import json
import logging
import numpy as np

from absl import app, flags

# flags should be:
# 1. path to json file that contains reference result
# 2. path to json file that contains acquired results invalidation process

logging.basicConfig(filename="./validationRaport.log", level=logging.INFO)


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


def meanErrorDifference(array1, array2):
    result = np.array(array1)
    reference = np.array(array2)
    return np.sum(np.abs(result - reference)) / np.mean(reference)


def standardDeviationError(array1, array2):
    result = np.array(array1)
    reference = np.array(array2)
    return np.std(np.abs(result - reference))


def RMSE(array1, array2):
    result = np.array(array1)
    reference = np.array(array2)
    return np.square(np.sum(np.power(result - reference, 2)) / result.size)


def maxError(array1, array2):
    result = np.array(array1)
    reference = np.array(array2)
    return np.max(np.abs(result - reference)) / result.size


def medianError(array1, array2):
    result = np.array(array1)
    reference = np.array(array2)
    return np.median(np.abs(result - reference)) / result.size


def minError(array1, array2):
    result = np.array(array1)
    reference = np.array(array2)
    return np.min(np.abs(result - reference)) / result.size


def main(argv):
    referencePath = argv[1]
    resultsPath = argv[2]

    logging.info(f"VALIDATION OF: {resultsPath}")

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
        FREQUENCIES_ARE_THE_SAME = ElementsAreArray(
            result["frequencies"], reference["frequencies"])

        if not FREQUENCIES_ARE_THE_SAME:
            # weird case
            pass
        else:
            logging.info(
                f"\tParameter Name: {parameter_name},\n" +
                f"\tMean error: {meanErrorDifference(result['values'], reference['values'])}\n" +
                f"\tStandard Deviation error: {standardDeviationError(result['values'], reference['values'])}\n" +
                f"\tRMSE: {RMSE(result['values'], reference['values'])}\n" +
                f"\tMin Error: {minError(result['values'], reference['values'])}\n" +
                f"\tMax Error: {maxError(result['values'], reference['values'])}\n" +
                f"\tMedian Error: {medianError(result['values'], reference['values'])}\n")

            # kurtoza


if __name__ == "__main__":
    app.run(main)

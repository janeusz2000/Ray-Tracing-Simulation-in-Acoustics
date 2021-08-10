import matplotlib as plt
import json
import csv
import math
import os

path = os.getcwd()
path = os.path.join(path, "validationTools")
path = os.path.join(path, "newReferenceCalculations")

print(path)


def convertCSVToValueDict(filePath: str) -> dict:
    headers = list()
    values = list()
    with open(filePath, newline='',) as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        count = 0
        for row in spamreader:
            if count == 8:
                index = 0
                for element in row:
                    if index == 18:
                        break
                    element = element.replace("%", " ")
                    element = element.replace(
                        "abs(acpr.p_s)^2 (kg^2/(m^2*s^4)) @ ", " ")
                    if "freq" in element:
                        element = element.replace("freq=", " ")
                        element += " Hz"
                    element = element.strip()
                    element = element.replace(" ", "")

                    headers.append(element)
                    index += 1
            if count > 8:
                values.append(row[0:18])
            count += 1
    output = dict()
    count = 0
    print(headers)
    for key in headers:
        mapValues = list()
        for valueList in values:
            mapValues.append(float(valueList[count]))
        output[key] = mapValues
        count += 1
    return output


def calculateDiffusionCoefficient(valueMap: dict) -> dict:
    output = dict()
    for key in valueMap.keys():
        if "Hz" in key:
            values = valueMap[key]
            alpha = math.pow(sum(values), 2)
            beta = sum([math.pow(value, 2) for value in values])
            gamma = (len(values) - 1) * beta
            output[key] = (alpha - beta) / gamma
    return output


def writeToJson(path: str, valueMap: dict) -> None:
    frequencies = [float(value[:-2]) for value in valueMap.keys()]
    name = "Acoustic Diffusion Coefficient"
    values = [valueMap[key] for key in valueMap.keys()]
    jsonDict = {'frequencies': frequencies,
                'name': name,
                'values': values}
    with open(path, 'w+') as f:
        f.writelines(json.dumps([jsonDict]))


if __name__ == "__main__":
    for filename in os.listdir(path):
        filenamePath = path + "/{}".format(filename)
        print(filenamePath)
        valueMap = convertCSVToValueDict(filenamePath)
        diffusionCoefficient = calculateDiffusionCoefficient(valueMap)
        referencePath = os.path.join(os.path.join(
            os.getcwd(), 'validationResults'), "Reference")
        if "line" in filename:
            referencePath = os.path.join(referencePath, 'line')
        else:
            referencePath = os.path.join(referencePath, 'surface')

        referencePath += "/{}".format(
            filename[:filename.rfind('_')][:-4] + ".json")

        writeToJson(referencePath, diffusionCoefficient)

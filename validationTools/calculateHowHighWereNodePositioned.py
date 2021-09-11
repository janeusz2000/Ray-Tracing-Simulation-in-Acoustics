import os
import csv

PATH = os.getcwd()
PATH = os.path.join(PATH, 'validationTools')
reference = os.path.join(PATH, 'MESresults')
newReferenceCalculations = os.path.join(PATH, 'newReferenceCalculations')


def calculateHowHigh(filepath: str):
    headers = list()
    values = list()
    with open(filepath, newline='') as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for count, row in enumerate(spamreader):
            if (count == 8):
                for index, element in enumerate(row):
                    if index == 3:
                        break
                    element = element.replace("%", " ")
                    element = element.replace(
                        "abs(acpr.p_s)^2 (kg^2/(m^2*s^4)) @ ", " ")
                    element = element.strip()
                    element.replace(" ", "")
                    headers.append(element)
            elif count > 8:
                values.append(row[0: 3])
    maximum = 0
    for value in values:
        maximum = max(float(value[2]), maximum)
    return maximum


if __name__ == "__main__":
    givenPath = os.path.join(
        reference, '2D_1m_200Hz_modulo7_30stopni_6n_out_powierzchnia.csv')
    print(f"simulation height: {calculateHowHigh(givenPath)}")

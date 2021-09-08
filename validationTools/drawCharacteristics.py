import json
import matplotlib.pyplot as plt
import os
import csv


PATH = os.getcwd()
validationToolsPath = os.path.join(PATH, 'validationTools')
diffusionReferenceDestination = os.path.join(
    validationToolsPath, 'diffusionReferenceGraphs')

validationResults = os.path.join(PATH, 'validationResults')
reference = os.path.join(validationResults, 'Reference')
lineReference = os.path.join(reference, 'line')
surfaceReference = os.path.join(reference, 'surface')

referencePlate = os.path.join(reference, 'referencePlate')
lineReferencePlate = os.path.join(referencePlate, 'line')
surfaceReferencePlate = os.path.join(referencePlate, 'surface')

modelLegend = {"1D_1m_modulo23_500Hz_46n_30stopni_5potega.json": "Model A",
               "1D_2m_modulo13_250Hz_9n_15stopni_5potega.json": "Model B",
               "2D_1m_200Hz_modulo7_30stopni_6n.json": "Model D",
               "2D_2m_6n_modulo7_200Hz_15stopni_5potega.json": "Model C",
               "1m_plyta.json": "Reference Plate 1 [m] x 1 [m]",
               "2m_plyta.json": "Reference Plate 2 [m] x 2 [m]"}


def readJson(file):
    with open(file, encoding='utf-8') as f:
        return json.loads(f.read())


def drawDiffusionCoefficient(ax, frequencies, values):
    plot, = ax.semilogx(frequencies, values)
    return plot


def setUpAx(ax):
    ax.set_title('MES Acoustic Diffusion Coefficient', fontsize=20)
    ax.set_xlabel('Frequency [Hz]', fontsize=18)
    ax.set_ylabel('Diffusion Coefficient [-]', fontsize=18)

    ax.grid()
    ax.set_ylim((0, 1))


def walkThroughAllReferenceDiffusors(paths, desiredName):
    fig, ax = plt.subplots(figsize=(14, 10))
    legend = list()
    plots = list()
    setUpAx(ax)
    frequencies = list()
    for path in paths:
        for filename in os.listdir(path):
            if filename.endswith('.json'):
                currentFile = readJson(os.path.join(path, filename))
                for acousticParameter in currentFile:
                    if acousticParameter['name'] == desiredName:
                        values = acousticParameter['values']
                        frequencies = acousticParameter['frequencies']
                        name = f"line {modelLegend[filename]} {acousticParameter['name']}" if "line" in path else f"surface {modelLegend[filename]} {acousticParameter['name']}"
                        legend.append(name)
                        plots.append(drawDiffusionCoefficient(ax,
                                                              frequencies=frequencies, values=values))
                        print(f"Model name: {name}")
                        print(
                            f"Values: {str([round(value, 2) for value in values])[1:-1]}")

    print(frequencies)
    ax.legend(plots, legend, loc='upper right', ncol=2)
    ax.set_xticks(frequencies)
    ax.set_xticklabels([int(freq) for freq in frequencies], fontsize=16)
    ax.set_yticklabels([0.0, 0.2, 0.4, 0.6, 0.8, 1.0], fontsize=16)
    fig.subplots_adjust(bottom=0.20)
    plt.savefig(os.path.join(
        diffusionReferenceDestination, "graphs.png"), format='png')


if __name__ == "__main__":
    walkThroughAllReferenceDiffusors(
        [surfaceReference],  'Normalized Acoustic Diffusion Coefficient')

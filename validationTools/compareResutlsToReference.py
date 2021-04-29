import json
from absl import app, flags

# flags should be:
# 1. path to json file that contains reference result
# 2. path to json file that contains acquired results invalidation process


def main(argv):
    referencePath = argv[1]
    resultsPath = argv[2]

    with open(referencePath) as jsonFile:
        reference = json.load(jsonFile)
    with open(resultsPath) as jsonFile:
        results = json.load(jsonFile)

    print("REFERENCE:")
    print(reference)
    print("RESULTS:")
    print(results)

if __name__ == "__main__":
    app.run(main)

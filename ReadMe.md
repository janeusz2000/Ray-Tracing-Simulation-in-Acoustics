# Ray-Tracing-Simulation-in-Acoustics

![Ray Tracing Simulation in Acoustics](https://i.imgur.com/TXBQoQc.jpg)

This project simulates the diffusion of energy from structures, specifically calculating the simulated diffusion coefficient and normalized diffusion coefficient for diffusers. The core of the project is a ray-tracing engine written in C++ that is controlled from the console. The method is developed for a more cost-effective and faster execution with similar results to the finite elements method.

## Overview

The Ray-Tracing-Simulation-in-Acoustics project aims to develop a new method for determining the acoustic diffusion coefficients of diffusers using ray-tracing techniques. The main features of the project include:

- A C++ ray-tracing engine that simulates the diffusion of energy from structures
- Console-based controls for the simulation engine
- Saving energy diffusion data to a JSON file
- A simple frontend application for visualizing the simulated rays and calculated acoustic coefficients
- MySQL database integration for storing and managing simulation data
- A Python script for scheduling multiple simulations on different models
- Unit tests for checking the functionality of the project

## How To Use

To use this project, you will need to follow a few steps:

1. Make sure you have installed:
   - [Bazel](https://bazel.build/)
   - [NPM](https://www.npmjs.com/)
   - [Webpack](https://webpack.js.org/)
   - [ThreeJS](https://threejs.org/)
   - [NodeMon](https://www.npmjs.com/package/nodemon)

2. Ensure that the center of your model is positioned at the middle in .obj file - [x, y, z] = (0, 0, 0).

3. Configure a MySQL database with the name "VALIDATIONS." Modify the local_ip, user, and password in the Python script `clearDatabase.py` located at `./validationTools/clearDatabase.py` and execute it.

4. Run a local frontend server in your browser with `nodemon app.js` in *./server/gui/app.js*.

Then you will have couple of options:

1. You can schedule multiple simulations for different models via Python scipt: `./validation.py`.

2. You can perform single custom simulation. In order to do that, create a .cpp file for building the binary, with specified properties of the simulation. It should look like this:

```cpp
#include "main/model.h"
#include "main/rayTracer.h"
#include "main/sceneManager.h"
#include "main/simulator.h"
#include "obj/generators.h"
#include <algorithm>
#include <string_view>

int main() {
  std::string_view path = "./models/simpleCube.obj"; // This projects supports diffusors given in .obj file only
  std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(path.data());
  trackers::saveModelToJson("./data", model.get());
  trackers::JsonPositionTracker tracker("./data");
  collectionRules::LinearEnergyCollection energyCollectionRules;
  std::vector<float> frequencies = {100, 200, 300, 400, 500};
  SimulationProperties properties(frequencies, &energyCollectionRules, 500, 37,
                                  1);
  SceneManager manager(model.get(), properties, &tracker);
  manager.run();
}
```
Then Add build instructions in the BUILD.bazel file
```bazel
cc_binary(
    name = "build_name",
    # Path to your build file
    srcs = [
        "ApplicationBuild/build.v1.1.cpp",
    ],
    deps = [
        ":utils",
    ],
)
```
In the console, run the simulation via:
```bash
bazel build --config=_gcc build_name
bazel-bin/build.v1.1
```
On the browser, you should see an image similar to the one below the title.
For more information and details about the project, please email me: 
mateusz.krzysztof.janik@gmail.com

# Ray-Tracing-Simulation-in-Acoustics

![Ray Tracing Simulation in Acoustics](https://i.imgur.com/TXBQoQc.jpg)

This project simulates the diffusion of energy from structures, specifically calculating the simulated diffusion coefficient and normalized diffusion coefficient for diffusers. The core of the project is a ray-tracing engine written in C++ that is controlled from the console. The method is developed for cheaper and faster execution with similar results to the finite elements method.

## Overview

The Ray-Tracing-Simulation-in-Acoustics project aims to develop a new method for determining the acoustic diffusion coefficients of diffusers using ray-tracing techniques. The main features of the project include:

- A C++ ray-tracing engine that simulates the diffusion of energy from structures
- Console-based controls for the simulation engine
- Saving energy diffusion data to a JSON file
- A simple frontend application for visualizing the simulated rays and calculated acoustic coefficients

## How To Use

To use this project, you will need to follow a few steps:

1. Make sure you have installed:
   - [Bazel](https://bazel.build/)
   - [NPM](https://www.npmjs.com/)
   - [Webpack](https://webpack.js.org/)
   - [ThreeJS](https://threejs.org/)
   - [NodeMon](https://www.npmjs.com/package/nodemon)

2. Make sure that the middle of your model is positioned at the middle of the simulation [x, y, z] = (0, 0, 0).

3. Run a local frontend server in your browser with `nodemon app.js` in *./server/gui/app.js*.

4. Create a .cpp file for building the binary, with specified properties of the simulation. It should look like this:

```cpp
#include "main/model.h"
#include "main/rayTracer.h"
#include "main/sceneManager.h"
#include "main/simulator.h"
#include "obj/generators.h"
#include <algorithm>
#include <string>
int main() {
  std::string path = "./models/simpleCube.obj";
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

5. Add build instructions in the BUILD.bazel file.
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

6. In the console, run the simulation via:
```bash
bazel build --config=_gcc build_name
bazel-bin/build.v1.1
```

7. On the browser, you should see an image similar to the one below the title.
For more information and details about the project, please email me: mateusz.krzysztof.janik@gmail.com


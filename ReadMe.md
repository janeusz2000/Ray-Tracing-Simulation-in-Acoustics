# **Acoustic Diffusion Coefficient Simulation via Ray-Tracing**
*by Mateusz Janik*


```diff
!! Under Development !!
```

<p align="center">
<img src="githubContent/demo.gif" alt="animated" />
</p>


## Description:
---
This is the development of the new method of simulating acoustic diffusion coefficient, which is supposed to be much faster than commonly used Finite Element Method, with similar results. The idea is to model how reflected acoustic energy is distributed across microphone different positions, which are arranged uniformly on the circumference's (as you can see on the gif: yellow spheres). Microphones properties, source position and simulation size are based on the recommendation of **ISO 17497-2:2012**.

---

## How To Use it:

---

To use this project, you will need to follow few steps.

1. Make sure you have installed:
- [Bazel](https://bazel.build/)
- [NPM](https://www.npmjs.com/)
- [Webpack](https://webpack.js.org/)
- [ThreeJS](https://threejs.org/)

1. Make sure that your model is positioned at the middle of simulation [x, y, z] = (0, 0, 0);

2. Run local server with opened *./gui/index.html* in your browser.

3. Create build file, with specified properties of the simulation. It should look like this:

```cpp
#include "main/model.h"
#include "main/rayTracer.h"
#include "main/simulator.h"
#include "obj/generators.h"

#include <algorithm>
#include <string>

// TODO: make it simpler
int main() {

  // Path to your model
  std::string path = "./models/monkey.obj";
  float sourcePower = 500; // [W]
  float frequency = 1e3;   // [Hz]

  // Number of total microphones positions.
  int numOfCollectors = 37;

  // Number of generated rays total is |numOfRaysAlongEachAxis|^2
  int numOfRaysAlongEachAxis = 15;

  // TODO: Hide this
  std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(path);
  trackers::saveModelToJson("./data", model.get());

  RayTracer rayTracer(model.get());
  trackers::PositionTracker positionTracker("./data");
  generators::PointSpeakerRayFactory pointSpeaker(numOfRaysAlongEachAxis,
                                                  sourcePower, model.get());
  generators::FakeOffseter rayOffseter;
  Simulator simulator(&rayTracer, model.get(), &pointSpeaker, &rayOffseter,
                      &positionTracker);

  std::vector<std::unique_ptr<objects::EnergyCollector>> collectors =
      buildCollectors(model.get(), numOfCollectors);
  exportCollectorsToJson(collectors, "./data/energyCollectors.json");

  std::vector<float> energies = simulator.run(frequency, numOfCollectors);
  positionTracker.saveAsJson();

  std::for_each(energies.begin(), energies.end(),
                [](float i) { std::cout << i << " "; });
  return 0;
}
```

5. Add build instructions in BUILD.bazel file.
```Go
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

6. In Bash run Simulation via:
```Bash
bazel build --config=_gcc build_name
bazel-bin/build.v1.1
```
7. Compile webpack with:
```Bash
npm start
```
8. in the browser you should see magic :smirk:

---

## Documentation:

## TODO List:
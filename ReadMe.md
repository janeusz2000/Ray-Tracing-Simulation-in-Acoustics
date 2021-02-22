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

and you should see all magic :smile: ;)
---

## Documentation:

### TODO List:
- [ ] fix normal at the edge
- [x] change EnergyPerFrequency to unordered_map<float, energies>>
- [x] split simulation properties to at leas two bundles of class
- [x] fix constructor of the simulationProperties
- [x] add comments about energyPerFrequency, what it represent
- [x] add exception msg for all cases
- [ ] explain what scene manager do
- [ ] consider changing name of the SceneManager
- [x] create dynamic reading object in gui
- [ ] export energies to gui
- [ ] create reference data
- [ ] export reference data to gui
- [ ] crate new tab inside of the gui to view results of the simulation
- [ ] diffusion coefficient per frequency on gui (x must be log10)
- [ ] create polar pattern kon every frequency
 (create list to choose frequency per polar pattern)
- [ ] calculate diffusion coefficient per frequency on gui
- [ ] create RayTracer interface (hit with one ray get s reflected + random ones
could produce another rays)
- [ ] talk with promoter about reference data from mes
- [ ] add multithreading support for speeding up experiments
- [ ] create architecture for running experiments
- [ ] create new ray-tracers to model wave phenomena
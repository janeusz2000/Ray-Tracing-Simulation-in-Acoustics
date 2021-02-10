#include "main/model.h"
#include "main/rayTracer.h"
#include "main/simulator.h"
#include "obj/generators.h"

#include <algorithm>

int main() {

  float modelSize = 1.0;
  float sourcePower = 500;
  float frequency = 1e3;

  int numOfCollectors = 37;
  int numOrFaysAlongEachAxis = 1e3;

  std::unique_ptr<Model> model = Model::NewReferenceModel(modelSize);
  RayTracer rayTracer(model.get());
  PositionTracker positionTracker("./data");
  generators::PointSpeakerRayFactory pointSpeaker(numOrFaysAlongEachAxis,
                                                  sourcePower, model.get());
  generators::FakeOffseter rayOffseter;
  Simulator simulator(&rayTracer, model.get(), &pointSpeaker, &rayOffseter,
                      &positionTracker);

  std::vector<float> energies = simulator.run(frequency, numOfCollectors);
  positionTracker.saveAsJson();

  std::for_each(energies.begin(), energies.end(),
                [](float i) { std::cout << i << " "; });
  return 0;
}

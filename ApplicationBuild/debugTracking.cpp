#include "main/model.h"
#include "main/rayTracer.h"
#include "main/resultsCalculation.h"
#include "main/sceneManager.h"
#include "main/simulator.h"
#include "obj/generators.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using EnergyPerTime = std::unordered_map<float, float>;
using Energy = std::vector<EnergyPerTime>;
using EnergyPerFrequency = std::unordered_map<float, Energy>;
using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

const int kSampleRate = 96e3;
const float sourcePower = 500; // [W]
const int numOfCollectors = 37;
const int numOfRaysSquared = 100;
const int numOfVisibleRaysSquared = 4;
const int maxTracking = 3;
// const std::vector<float> frequencies = {
//     100,  200,  300,  400,  500,  630,  800,  1000,  1250,  1600,
//     2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500, 16000};
const std::vector<float> frequencies = {1000};
float kDefaultModelSize = 1.0;

// std::string_view modelPath =
//     "./validationDiffusors/2D_2m_6n_modulo7_200Hz_15stopni_5potega.obj";
std::string_view serverDataPath = "./server/data";

// ARGS MUST CONTAIN:
// frequency that will be used in simulation

std::unique_ptr<Model>
getInfiniteLoopRayTracingModel(float distanceBetweenPlates, float size) {
  auto getClockWiseOrigins = [&](float size) -> std::vector<core::Vec3> {
    return {core::Vec3(-size, size, 0), core::Vec3(size, size, 0),
            core::Vec3(size, -size, 0), core::Vec3(-size, -size, 0)};
  };
  auto getTriangles = [&](std::vector<core::Vec3> &clockWiseOrigins)
      -> std::vector<objects::TriangleObj> {
    return {objects::TriangleObj(clockWiseOrigins[1], clockWiseOrigins[2],
                                 clockWiseOrigins[3]),
            objects::TriangleObj(clockWiseOrigins[3], clockWiseOrigins[0],
                                 clockWiseOrigins[1])};
  };

  std::vector<core::Vec3> firstLayer = getClockWiseOrigins(size);
  std::vector<objects::TriangleObj> firstLayerTriangles =
      getTriangles(firstLayer);

  std::vector<core::Vec3> secondLayer = getClockWiseOrigins(size);
  std::for_each(secondLayer.begin(), secondLayer.end(),
                [&](core::Vec3 &position) {
                  position += distanceBetweenPlates * core::Vec3::kZ;
                });

  std::vector<objects::TriangleObj> secondLayerTriangles =
      getTriangles(secondLayer);

  firstLayerTriangles.insert(firstLayerTriangles.end(),
                             secondLayerTriangles.begin(),
                             secondLayerTriangles.end());
  return std::make_unique<Model>(firstLayerTriangles);
};
int main(int argc, char *argv[]) {

  trackers::startSimulation();
  //   const std::vector<float> frequencies = {std::stof(argv[1])};
  trackers::DataExporter dataExporter;

  //   std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(modelPath);
  // std::unique_ptr<Model> model =
  //     getInfiniteLoopRayTracingModel(/*distanceBetween=*/8, /*size=*/0.3);
  std::unique_ptr<Model> model = Model::NewReferenceModel(1.0f);
  // generators::CustomPointRayFactory customRayFactory(
  //     /*origin=*/core::Vec3(0, 0, 2),
  //     /*direction=*/core::Vec3(0, 0, -1),
  //     /*energy=*/500);

  trackers::JsonSampledPositionTracker positionTracker(
      serverDataPath, numOfRaysSquared, numOfVisibleRaysSquared);
  //     trackers::SimplePositionTracker positionTracker;
  trackers::CollectorsTrackerToJson collectorsTracker;

  collectionRules::NonLinearEnergyCollection energyCollectionRules;
  BasicSimulationProperties basicProperties(
      frequencies, sourcePower, numOfCollectors, numOfRaysSquared, maxTracking);

  SimulationProperties properties(&energyCollectionRules, basicProperties);
  // SimpleFourSidedReflectionEngine reflectionEngine;
  FakeReflectionEngine reflectionEngine;
  SceneManager manager(model.get(), properties, &positionTracker,
                       &collectorsTracker, &reflectionEngine);

  // XAxisCollectorBuilder collectorBuilder;
  // YAxisCollectorBuilder collectorBuilder;
  // DoubleAxisCollectorBuilder collectorBuilder;
  GeometricDomeCollectorBuilder collectorBuilder;
  std::unordered_map<float, Collectors> mapOfCollectors =
      manager.newRun(&collectorBuilder);

  dataExporter.saveModelToJson(serverDataPath, model.get(),
                               /*referenceModel=*/false);

  //   positionTracker.switchToReferenceModel();
  //   std::unique_ptr<Model> referenceModel =
  //       Model::NewReferenceModel(model->sideSize());

  //   SceneManager referenceManager(referenceModel.get(), properties,
  //                                 &positionTracker, &collectorsTracker,
  //                                 &reflectionEngine);

  //   std::unordered_map<float, Collectors> mapOfReferenceCollectors =
  //       referenceManager.run(&collectorBuilder);
  //   dataExporter.saveModelToJson(serverDataPath, referenceModel.get(),
  //                                /*referenceModel=*/true);
  std::cout << positionTracker << std::endl;
  trackers::endSimulation();
}

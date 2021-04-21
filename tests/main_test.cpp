#include "main/model.h"
#include "main/rayTracer.h"
#include "main/simulator.h"
#include "obj/generators.h"
#include "gtest/gtest.h"

using core::Ray;
using core::RayHitData;
using core::Vec3;
using generators::FakeOffseter;
using generators::PointSpeakerRayFactory;

class MainTest : public ::testing::Test {
public:
  MainTest()
      : sourcePower_(500), frequency_(1e3), numOfCollectors_(37),
        numOfRayAlongEachAxis_(1) {
    model_ = Model::NewReferenceModel(1.0);

    dataExporter_.saveModelToJson("/tmp", model_.get());
  }

protected:
  float sourcePower_;
  float frequency_;
  int numOfCollectors_;
  int numOfRayAlongEachAxis_;
  std::unique_ptr<Model> model_;
  trackers::DataExporter dataExporter_;
};

TEST_F(MainTest, buildingSimulation) {

  RayTracer rayTracer(model_.get());
  trackers::JsonPositionTracker positionTracker("/tmp");
  PointSpeakerRayFactory pointSpeaker(numOfRayAlongEachAxis_, sourcePower_,
                                      model_.get());
  collectionRules::LinearEnergyCollection energyCollectionRules;

  FakeOffseter rayOffseter;
  Simulator simulator(&rayTracer, model_.get(), &pointSpeaker, &rayOffseter,
                      &positionTracker, &energyCollectionRules);

  std::vector<std::unique_ptr<objects::EnergyCollector>> collectors =
      buildCollectors(model_.get(), numOfCollectors_);

  int maxTracking = 12;
  simulator.run(frequency_, &collectors, maxTracking);
  // positionTracker.saveAsJson();
  // FAIL();
}

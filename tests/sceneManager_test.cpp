#include "main/model.h"
#include "main/sceneManager.h"
#include "gtest/gtest.h"

#include <memory>

class SceneManagerSimple : ::testing::Test {
public:
  SceneManagerSimple() {
    float modelSize = 1.0; // [m]
    model_ = Model::NewReferenceModel(modelSize);
  };

protected:
  std::unique_ptr<Model> model_;
};
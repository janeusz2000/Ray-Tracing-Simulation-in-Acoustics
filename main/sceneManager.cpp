#include "main/sceneManager.h"

// TODO: Change all doubles to float

bool SceneManager::loadEnergyColellectors(const std::vector<objects::Object *> *energyCollectorsPtr)
{
    // TODO: load energy collector implementation
    return false;
}

bool SceneManager::loadSample(std::vector<objects::Object *> *samplePtr)
{
    // TODO: load sampple implementation
    return false;
}

bool SceneManager::loadTestSample(std::vector<objects::Object *> *testSample)
{

    // a) Vec3(-0.5, -0.5, 0)
    // b) Vec3(0.5, -0.5, 0)
    // c) Vec3(-0.5, 0.5, 0)
    // d) Vec3(0.5, 0.5, 0)

    core::Vec3 k0(-_sampleSize, -_sampleSize, 0);
    core::Vec3 k1(_sampleSize, -_sampleSize, 0);
    core::Vec3 k2(-_sampleSize, _sampleSize, 0);
    core::Vec3 k3(_sampleSize, _sampleSize, 0);

    objects::TriangleObj temp1(k0, k1, k2));
    objects::TriangleObj temp2(k0, k1, k2));

    std::vector<objects::Object *> tempTestSample;

    *testSample = tempTestSample;
    if (testSample = nullptr)
        return false;
    return true;
}

bool SceneManager::loadSphereWall(objects::Object *SphereWall)
{
    // TODO: load SPhereWall
    return false;
}

bool SceneManager::importSample(std::string_view objPath)
{
    // TODo: import sample via object reader
    return false;
}

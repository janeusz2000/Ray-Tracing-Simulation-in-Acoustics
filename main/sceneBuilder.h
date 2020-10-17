#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include "constants.h"
#include "core/vec3.h"
#include "core/ray.h"
#include "obj/generators.h"
#include "obj/objects.h"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

// This class is creating all necessary objects for simulation,
// assign them to the right positions and give ownership 
// of those objects to the Simulator class, where simulation will be 
// performed.

class SceneBuilder
{
    public:
    SceneBuilder() {};

    // Construct Sceneshould move ownership of the objects to the class Simulator
    // 
    // returned vector needs to have;
    // - preddefined size
    // - all energycollectors
    // - should use private method build collector for situating it at the right position
    bool buildEnergyColellectors(const std::vector<objects::Object*>* energyCollectorsPtr); 

    // Sample method should return imported sample from object reader that will be in the future and move owenership
    bool buildSample(std::vector<objects::Object*> *samplePtr);   

    // THis just return test sample which containt two triangleObj that are making 
    // perfect square thogether.
    bool buildTestSample(std::vector<objects::Object*> *testSample);
    
    // should return sphere wall object. If SPhere wall is hit only, energy is collected
    bool sphereWall(objects::Object* SphereWall);
    
    // import sample via ObjReader class. 
    void importSample(std::string_view objPath); 
};


#endif

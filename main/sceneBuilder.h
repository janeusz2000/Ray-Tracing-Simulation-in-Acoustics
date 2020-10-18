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
// assign them to the given pointer from class Simulator

class SceneBuilder
{
    public:
    SceneBuilder() {};

    // builds std::vector<Objects*> with Energy collectors  and assign it to the given pointer. 
    bool buildEnergyColellectors(const std::vector<objects::Object*>* energyCollectorsPtr); 
    // This method should build vector<Object*> with usage of ObjectReader class. Object Reader
    // is translateing .obj files into TriangleObj.
    bool buildSample(std::vector<objects::Object*> *samplePtr);   

    // buildTestSample method is assigning testSample to given pointer which containt two triangleObj that are making 
    // perfect square thogether.
    bool buildTestSample(std::vector<objects::Object*> *testSample);
    
    // should return sphere wall object. If SPhere wall is hit only, energy is collected
    bool sphereWall(objects::Object* SphereWall);
    
    // import sample via ObjReader class. 
    void importSample(std::string_view objPath); 
};


#endif

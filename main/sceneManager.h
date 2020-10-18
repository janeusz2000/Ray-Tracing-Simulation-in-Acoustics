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

class SceneManager
{
    public:
    SceneManager() {};

    // builds std::vector<Objects*> with Energy collectors  and assign it to the given pointer. 
    // Returns true if it was successful
    bool loadEnergyColellectors(const std::vector<objects::Object*>* energyCollectorsPtr); 

    // This method builds vector<Object*> with usage of ObjectReader class. Object Reader
    // is translateing .obj files into TriangleObj. Returns true if successful.
    bool loadSample(std::vector<objects::Object*> *samplePtr);   

    // buildTestSample method is assigning testSample to given pointer which containt two 
    // triangleObj that are making perfect square thogether.
    // Returns true if successful.
    bool loadTestSample(std::vector<objects::Object*> *testSample);
    
    // assigns single sphere wall object to given pointer. Returns true if successful.  
    bool loadSphereWall(objects::Object* SphereWall);
    
    // import sample via ObjReader class. Returns true if successful.
    bool importSample(std::string_view objPath); 

    private:
    // Disallow copy and assign
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

};


#endif

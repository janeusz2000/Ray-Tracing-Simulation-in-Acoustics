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


class SceneBuilder
{
    public:
    SceneBuilder() : _collectorCurrentId(0) {};

    // Construct Sceneshould move ownership of the objects to the class Simulator
    // 
    // returned vector needs to have;
    // - preddefined size
    // - all energycollectors
    // - should use private method build collector for situating it at the right position
    std::vector<std::unique_ptr<objects::Object>> BuildEnergyColellectors(); 

    // Sample method should return imported sample from object reader that will be in the future and move owenership
    std::vector<std::unique_ptr<objects::Object>> BuildSample();   

    // test sample should return vector of the two triangleObjs creating square that is situated at Vec3(0, 0, 0) in the middle;
    // it also should have SphereWall object at the end. 
    // Point Source has origin at Vec3(0, 0, 4)
    //
    // Two triangle objects are making perfect square at:
    //
    // a) Vec3(-0.5, -0.5, 0)
    // b) Vec3(0.5, -0.5, 0)
    // c) Vec3(-0.5, 0.5, 0)
    // d) Vec3(0.5, 0.5, 0)
    //
    // area of the testSample is equal to 1 m^2
    //
    // Square area that generated rays are hiting is equal to the 4/3 * 4/3 = 16 / 9
    // This comes from the triangle laws;
    // Point source origin is at Vec3(0, 0, 4) and it's calculateing its direction at the same
    // square as sample test, but lifted up by Vec3(0, 0, 1)
    // from triangle law we get (3 / 0.5)^2  [ (Vec3(0, 0, 4) - Vec3(0, 0, 1) / ( a / 2))^2 ]
    // which is equal to 4/3 * 4/3 = 16 /9
    std::vector<std::unique_ptr<objects::Object>> BuildTestSample();

    // should return sphere wall object. If SPhere wall is hit only, energy is collected
    std::unique_ptr<objects::Object> sphereWall();

    // import sample via ObjReader class. 
    void importSample(std::string_view objPath); 
    


    private:
    void buildCollector(); // This should build EnergyCollector at the right position based on the 
    size_t _collectorCurrentId;
    
};


#endif

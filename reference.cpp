
#include <memory>
#include <string_view>
#include <tuple>
#include <vector>

class Vec3 {};

struct RandomRayOffseter {
public:
  Ray offsetRay(const Ray &ray) {
    return Ray(ray.origin(), ray.direction() + Vec3(getNextAxisOffset(),
                                                    getNextAxisOffset(), 0));
  }

protected:
  virtual float getNextAxisOffset() const = 0;
};

// Abstract class of the generator
class RayFactory {
public:
  // generates ray with properties determinated by each class
  // example: Speaker source is a membrane that is generating
  // rays that have origins where membrane is situated and direction
  // parpendicular to surface where membrane is situated.
  // D: questions:
  //    a) how many distinct "classes" you can have?
  //    b) why do you take the OffsetGenerator as a parameter? do you want to
  //    swap
  //      the parameter mid simulation?
  // A:
  //    a) In teh future i could have multipule sources at once
  //    b) I dont want to change offsetGenerator mid simulation,
  //    but for now I cannot determinate which one I will be using,
  //    so i want to have this choice.
  virtual bool genRay(Ray *ray) = 0;
};

// generates rays with origin at PointSource origin
// and direction along Z axes down, with offset on XY
// determinated by sampleSize until numOfRay number is reached
class PointSpeakerRayFactory : RayFactory {
public:
  PointSpeakerRayFactory(int numOfRays, float simulationRadius);
  bool genRay(Ray *ray) override;
};

class Model {
public:
  // Model that simulation will be performend on
  std::vector<TriangleObj *> triangles() const;
  float simulationRadius() const;

  static std::unique_ptr<Model> NewLoadFromObjectFile(std::string_view path);
  static std::unique_ptr<Model> NewReferenceModel(float sideSize);
};

class RayTracer {
public:
  RayTracer(Model *model);

  // TODO: write comment about hit data, that its required
  // return
  enum class TraceResult { HIT_TRIANGLE, WENT_OUTSIDE_OF_SIMULATION_SPACE };
  TraceResult rayTrace(const Ray &ray, RayHitData *hitData);
};

std::vector<std::unique_ptr<EnergyCollector>>
BuildCollectors(const Model &model, int numCollectors);

class Simulator {
public:
  Simulator(RayTracer *tracer, Model *model, RayFactory *source,
            RandomRayOffseter *offsetter);

  // While it is possible, invoke RayTrace method with ray given
  // by PointSource
  // D: this sentence seems unfinished. We talked about side-effects that they
  // make code much harder to understand. how am I supposed to get results back
  // after I run this? why not simply returning some results?
  std::vector<float> run(float frequency, int numCollectors) {
    std::vector<std::unique_ptr<EnergyCollector>> collectors =
        buildCollector(...);
  }
};

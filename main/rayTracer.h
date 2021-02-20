#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "core/ray.h"
#include "core/vec3.h"
#include "main/model.h"
#include "obj/objects.h"

class RayTracer {
public:
  RayTracer(ModelInterface *model) : model_(model){};

  friend std::ostream &operator<<(std::ostream &os, const RayTracer &rayTracer);

  enum class TraceResult { HIT_TRIANGLE, WENT_OUTSIDE_OF_SIMULATION_SPACE };
  // |hitData| is modified to hold information where ray hit the triangle,
  // or where it went outside the simulation
  [[nodiscard]] TraceResult rayTrace(const core::Ray &ray, float frequency,
                                     core::RayHitData *hitData);
  core::Ray getReflected(core::RayHitData *hitdata) const;

private:
  ModelInterface *model_;
};

#endif
#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "core/classUtlilities.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "main/model.h"
#include "obj/objects.h"

class RayTracer : public Printable {
public:
  RayTracer(ModelInterface *model) : model_(model){};

  enum class TraceResult { HIT_TRIANGLE, WENT_OUTSIDE_OF_SIMULATION_SPACE };
  // |hitData| is modified to hold information where ray hit the triangle,
  // or where it went outside the simulation
  [[nodiscard]] TraceResult rayTrace(const core::Ray &ray, float frequency,
                                     core::RayHitData *hitData);
  core::Ray getReflected(core::RayHitData *hitdata) const;
  void printItself(std::ostream &os) const noexcept override;

private:
  ModelInterface *model_;
};

#endif
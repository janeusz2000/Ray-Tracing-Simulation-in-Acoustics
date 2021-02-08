#include "main/rayTracer.h"

RayTracer::TraceResult RayTracer::rayTrace(const core::Ray &ray,
                                           float frequency,
                                           core::RayHitData *hitData) {
  bool hit = false;
  core::RayHitData closestHitData;
  for (objects::TriangleObj triangle : model_->triangles()) {
    if (triangle.hitObject(ray, frequency, hitData)) {
      hit = true;
      if (closestHitData.time > hitData->time) {
        closestHitData = *hitData;
      }
    }
  }
  if (hit) {
    *hitData = closestHitData;
    return TraceResult::HIT_TRIANGLE;
  }
  return TraceResult::WENT_OUTSIDE_OF_SIMULATION_SPACE;
}

// Returns reflection Ray from hit point stored in |hitData|
// http://paulbourke.net/geometry/reflected/
core::Ray RayTracer::getReflected(core::RayHitData *hitData) const {

  core::Vec3 newDirection =
      hitData->direction() -
      2 * hitData->normal() *
          hitData->direction().scalarProduct(hitData->normal());

  return core::Ray(hitData->collisionPoint(), newDirection, hitData->energy());
}

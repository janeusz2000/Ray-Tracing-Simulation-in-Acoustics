#include "core/ray.h"
#include "core/vec3.h"
#include "main/model.h"
#include "obj/objects.h"

class RayTracer {
public:
  RayTracer(ModelInterface *model);
  enum class TraceResult { HIT_TRIANGLE, WENT_OUTSIDE_OF_SIMULATION_SPACE };
  // hitData or ray cannot be nullptr. On every recursive loop hitData is
  // modified - current state of RayHitData passed to method does not matter.
  [[nodiscard]] TraceResult rayTrace(const core::Ray &ray,
                                     core::RayHitData *hitData);
};
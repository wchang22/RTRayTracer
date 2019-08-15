#include "intersectable.h"
#include "aabb.h"

bool Intersectable::is_in_aabb(const AABB& aabb) const
{
  for (int axis = 0; axis < 3; axis++) {
    vec2 bounds = get_bounds(static_cast<Axis>(axis));
    vec2 aabb_bounds = aabb.get_bounds(static_cast<Axis>(axis));

    if (bounds[0] < aabb_bounds[0] || bounds[1] > aabb_bounds[1]) {
      return false;
    }
  }

  return true;
}

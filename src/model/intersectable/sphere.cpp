#include "sphere.h"

Sphere::Sphere(const vec3& center, float radius)
  : center(center), radius(radius)
{
}

Intersectable::Type Sphere::get_type() const
{
  return Type::Sphere;
}

vec3 Sphere::get_center() const {
  return center;
}

vec2 Sphere::get_bounds(Axis axis) const {
  int axis_int = static_cast<int>(axis);
  return vec2(center[axis_int] - radius, center[axis_int] + radius);
}

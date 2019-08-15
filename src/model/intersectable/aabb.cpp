#include "aabb.h"

AABB::AABB(const vec3& center, const vec3& lengths)
  : center(center, 0.0f), lengths(lengths, 0.0f)
{
}

Intersectable::Type AABB::get_type() const
{
  return Type::AABB;
}

vec3 AABB::get_center() const
{
  return center;
}

vec2 AABB::get_bounds(Axis axis) const
{
  int axis_int = static_cast<int>(axis);
  float half_length = lengths[axis_int] / 2.0f;
  return vec2(center[axis_int] - half_length, center[axis_int] + half_length);
}

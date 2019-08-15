#ifndef AABB_H
#define AABB_H

#include "intersectable.h"

struct AABB : public Intersectable
{
  AABB(const vec3& center, const vec3& lengths);

  Type get_type() const override;
  vec3 get_center() const override;
  vec2 get_bounds(Axis axis) const override;

  vec4 center;
  vec4 lengths;
};

#endif // AABB_H

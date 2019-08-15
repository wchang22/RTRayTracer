#ifndef SPHERE_H
#define SPHERE_H

#include "intersectable.h"

struct Sphere : public Intersectable {
  Sphere(const vec3& center, float radius);

  Type get_type() const override;
  vec3 get_center() const override;
  vec2 get_bounds(Axis axis) const override;

  vec3 center;
  float radius;
};

#endif // SPHERE_H

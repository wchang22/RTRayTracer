#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "intersectable.h"

struct Triangle : public Intersectable
{
public:
  Triangle(vec3 v1, vec3 v2, vec3 v3);

  Type get_type() const override;
  vec3 get_center() const override;
  vec2 get_bounds(Axis axis) const override;

  vec3 vertices[4];
};

#endif // TRIANGLE_H

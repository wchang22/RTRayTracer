#include "triangle.h"

#include <iterator>

Triangle::Triangle(vec3 v1, vec3 v2, vec3 v3)
{
  vertices[0] = v1;
  vertices[1] = v2;
  vertices[2] = v3;
}

Intersectable::Type Triangle::get_type() const
{
  return Type::Triangle;
}

vec3 Triangle::get_center() const {
  return (vertices[0] + vertices[1] + vertices[2]) / 3.0f;
}

vec2 Triangle::get_bounds(Axis axis) const {
  float min = std::min(vertices[0][static_cast<int>(axis)],
              std::min(vertices[1][static_cast<int>(axis)],
                       vertices[2][static_cast<int>(axis)]));
  float max = std::max(vertices[0][static_cast<int>(axis)],
              std::max(vertices[1][static_cast<int>(axis)],
                       vertices[2][static_cast<int>(axis)]));
  return vec2(min, max);
}

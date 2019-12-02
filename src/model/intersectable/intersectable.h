#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include <glm/glm.hpp>

using namespace glm;

struct AABB;

class Intersectable
{
public:
  enum class Axis {
    X, Y, Z
  };
  enum class Type : int {
    Triangle, AABB
  };

  virtual ~Intersectable() = default;

  virtual Type get_type() const = 0;
  virtual vec3 get_center() const = 0;
  virtual vec2 get_bounds(Axis axis) const = 0;
  virtual bool is_in_aabb(const AABB& aabb) const;
};

#endif // INTERSECTABLE_H

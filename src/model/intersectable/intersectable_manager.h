#ifndef INTERSECTABLEMANAGER_H
#define INTERSECTABLEMANAGER_H

#include <vector>
#include <glm/glm.hpp>

#include "triangle.h"
#include "aabb.h"
#include "acceleration/bvh.h"

using namespace glm;

class IntersectableManager
{
public:
  IntersectableManager();
  ~IntersectableManager();

  struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
  };

  void add_triangle(Triangle&& triangle, Material&& material);
  void finalize();

private:
  std::unique_ptr<BVH> build_bvh();

  unsigned int intersectables, materials;
  std::vector<std::pair<Triangle, Material>> triangles;
  std::unique_ptr<BVH> tree;
};

#endif // INTERSECTABLEMANAGER_H

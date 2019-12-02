#ifndef INTERSECTABLEMANAGER_H
#define INTERSECTABLEMANAGER_H

#include <vector>
#include <glm/glm.hpp>

#include "sphere.h"
#include "triangle.h"
#include "aabb.h"
#include "acceleration/kdtree.h"

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
  void add_sphere(Sphere&& sphere, Material&& material);
  void add_aabb(AABB&& aabb, Material&& material);
  void finalize();

private:
  std::unique_ptr<KDTree> build_kd_tree();

  unsigned int intersectables, materials;
  std::vector<std::pair<Triangle, Material>> triangles;
  std::vector<std::pair<Sphere, Material>> spheres;
  std::vector<std::pair<AABB, Material>> aabbs;
  std::unique_ptr<KDTree> tree;
};

#endif // INTERSECTABLEMANAGER_H

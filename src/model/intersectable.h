#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include <vector>
#include <glm/glm.hpp>

using namespace glm;

class Intersectable
{
public:
  Intersectable();
  ~Intersectable();

  struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
  };

  struct Triangle {
    vec3 vertices[3];
  };

  struct Sphere {
    vec3 center;
    float radius;
  };

  struct AxisAlignedBox {
    vec3 center;
    float x;
    float y;
    float z;
  };

  void add_triangle(Triangle&& triangle, Material&& material);
  void add_sphere(Sphere&& sphere, Material&& material);
  void add_axis_aligned_box(AxisAlignedBox&& aabb, Material&& material);
  void finalize();

private:
  unsigned int intersectables, num_intersectables, materials;
  std::vector<std::pair<Triangle, Material>> triangles;
  std::vector<std::pair<Sphere, Material>> spheres;
  std::vector<std::pair<AxisAlignedBox, Material>> aabbs;
};

#endif // INTERSECTABLE_H

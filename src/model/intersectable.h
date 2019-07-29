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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
  };

  struct Triangle {
    vec3 vertices[3];
  };

  struct Sphere {
    vec3 center;
    float radius;
  };

  void add_triangle(Triangle&& triangle, Material&& material);
  void add_sphere(Sphere&& sphere, Material&& material);
  void finalize();

private:
  unsigned int intersectables, num_intersectables, materials;
  std::vector<std::pair<Triangle, Material>> triangles;
  std::vector<std::pair<Sphere, Material>> spheres;
};

#endif // INTERSECTABLE_H

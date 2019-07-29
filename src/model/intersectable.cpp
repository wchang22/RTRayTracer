#include "intersectable.h"

#include <glad/glad.h>

Intersectable::Intersectable()
{
  glGenBuffers(1, &intersectables);
  glGenBuffers(1, &materials);
  glGenBuffers(1, &num_intersectables);
}

Intersectable::~Intersectable()
{
  glDeleteBuffers(1, &intersectables);
  glDeleteBuffers(1, &materials);
  glDeleteBuffers(1, &num_intersectables);
}

void Intersectable::add_triangle(Intersectable::Triangle&& triangle, Intersectable::Material&& material)
{
  triangles.emplace_back(std::move(triangle), std::move(material));
}

void Intersectable::add_sphere(Intersectable::Sphere&& sphere, Intersectable::Material&& material)
{
  spheres.emplace_back(std::move(sphere), std::move(material));
}

void Intersectable::finalize()
{
  const int num_objects[2] = { static_cast<int>(spheres.size()),
                               static_cast<int>(triangles.size()) };

  glBindBuffer(GL_UNIFORM_BUFFER, num_intersectables);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof (int), num_objects, GL_STATIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 3, num_intersectables);

  std::vector<vec4> intersectable_data;
  std::vector<vec4> material_data;
  intersectable_data.reserve((spheres.size() + triangles.size()) * 3);
  material_data.reserve((spheres.size() + triangles.size()) * 3);

  for (unsigned int i = 0; i < spheres.size(); i++) {
    const auto& [sphere, material] = spheres[i];
    intersectable_data.emplace_back(vec4(sphere.center, sphere.radius));
    intersectable_data.insert(intersectable_data.end(), {{}, {}});
    material_data.emplace_back(vec4(material.ambient, 0.0));
    material_data.emplace_back(vec4(material.diffuse, 0.0));
    material_data.emplace_back(vec4(material.specular, material.shininess));
  }

  for (unsigned int i = static_cast<unsigned int>(spheres.size());
     i < triangles.size() + spheres.size(); i++) {
    const auto& [triangle, material] = triangles[i - spheres.size()];
    vec3 e1 = triangle.vertices[1] - triangle.vertices[0];
    vec3 e2 = triangle.vertices[2] - triangle.vertices[0];
    vec3 n = glm::cross(e1, e2);
    intersectable_data.emplace_back(vec4(triangle.vertices[0], e2.x));
    intersectable_data.emplace_back(vec4(n, e2.y));
    intersectable_data.emplace_back(vec4(e1, e2.z));
    material_data.emplace_back(vec4(material.ambient, 0.0));
    material_data.emplace_back(vec4(material.diffuse, 0.0));
    material_data.emplace_back(vec4(material.specular, material.shininess));
  }

  constexpr GLenum buffer_type = GL_SHADER_STORAGE_BUFFER;

  glBindBuffer(buffer_type, intersectables);
  glBufferStorage(buffer_type,
                  static_cast<long>((spheres.size() + triangles.size()) * 3 * sizeof (vec4)),
                  intersectable_data.data(), 0);
  glBindBufferBase(buffer_type, 4, intersectables);

  glBindBuffer(buffer_type, materials);
  glBufferStorage(buffer_type,
                  static_cast<long>((spheres.size() + triangles.size()) * 3 * sizeof (vec4)),
                  material_data.data(), 0);
  glBindBufferBase(buffer_type, 5, materials);

  glBindBuffer(buffer_type, 0);
}

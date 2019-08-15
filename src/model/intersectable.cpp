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

void Intersectable::add_axis_aligned_box(Intersectable::AxisAlignedBox&& aabb,
                                         Intersectable::Material&& material)
{
  aabbs.emplace_back(std::move(aabb), std::move(material));
}

void Intersectable::finalize()
{
  const std::vector<int> num_objects = {
    static_cast<int>(spheres.size()),
    static_cast<int>(triangles.size()),
    static_cast<int>(aabbs.size())
  };
  const size_t total_size = spheres.size() + triangles.size() + aabbs.size();
  constexpr size_t intersectable_stride = 3;
  constexpr size_t material_stride = 3;

  glBindBuffer(GL_UNIFORM_BUFFER, num_intersectables);
  glBufferData(GL_UNIFORM_BUFFER, static_cast<long>(num_objects.size() * sizeof (int)),
               num_objects.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 3, num_intersectables);

  std::vector<vec4> intersectable_data;
  std::vector<vec4> material_data;
  intersectable_data.reserve(total_size * intersectable_stride);
  material_data.reserve(total_size * intersectable_stride);

  const auto add_material = [&material_data](const Material& material) {
    material_data.emplace_back(vec4(material.albedo, 0.0));
    material_data.emplace_back(vec4(material.metallic, material.roughness, material.ao, 0.0));

    vec3 f0 = glm::mix(vec3(0.04f), material.albedo, material.metallic);
    vec3 reflectance = (f0 + (vec3(1.0f) - f0) * pow(0.5f, 5.0f)) * (1.0f - material.roughness);
    material_data.emplace_back(vec4(reflectance, 0.0));
  };

  for (const auto& [sphere, material] : spheres) {
    intersectable_data.emplace_back(vec4(sphere.center, sphere.radius * sphere.radius));
    intersectable_data.insert(intersectable_data.end(), {{}, {}});
    add_material(material);
  }

  for (const auto& [triangle, material] : triangles) {
    vec3 e1 = triangle.vertices[1] - triangle.vertices[0];
    vec3 e2 = triangle.vertices[2] - triangle.vertices[0];
    vec3 n = glm::cross(e1, e2);
    intersectable_data.emplace_back(vec4(triangle.vertices[0], e2.x));
    intersectable_data.emplace_back(vec4(n, e2.y));
    intersectable_data.emplace_back(vec4(e1, e2.z));
    add_material(material);
  }

  for (const auto& [aabb, material] : aabbs) {
    intersectable_data.emplace_back(vec4(aabb.center - aabb.lengths / 2.0f, 0.0));
    intersectable_data.emplace_back(vec4(aabb.center + aabb.lengths / 2.0f, 0.0));
    intersectable_data.emplace_back();
    add_material(material);
  }

  constexpr GLenum buffer_type = GL_SHADER_STORAGE_BUFFER;

  glBindBuffer(buffer_type, intersectables);
  glBufferStorage(buffer_type,
                  static_cast<long>(total_size * intersectable_stride * sizeof (vec4)),
                  intersectable_data.data(), 0);
  glBindBufferBase(buffer_type, 4, intersectables);

  glBindBuffer(buffer_type, materials);
  glBufferStorage(buffer_type,
                  static_cast<long>(total_size * material_stride * sizeof (vec4)),
                  material_data.data(), 0);
  glBindBufferBase(buffer_type, 5, materials);

  glBindBuffer(buffer_type, 0);
}

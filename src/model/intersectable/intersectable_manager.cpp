#include "intersectable_manager.h"

#include <glad/glad.h>
#include <algorithm>
#include <memory>

IntersectableManager::IntersectableManager()
{
  glGenBuffers(1, &intersectables);
  glGenBuffers(1, &materials);
}

IntersectableManager::~IntersectableManager()
{
  glDeleteBuffers(1, &intersectables);
  glDeleteBuffers(1, &materials);
}

void IntersectableManager::add_triangle(Triangle&& triangle, Material&& material)
{
  if (triangles.size() > 2000) {
    return;
  }
  triangles.emplace_back(std::move(triangle), std::move(material));
}

void IntersectableManager::finalize()
{
  const size_t total_size = triangles.size();
  constexpr size_t intersectable_stride = 3;
  constexpr size_t material_stride = 3;

  std::vector<vec4> intersectable_data;
  std::vector<vec4> material_data;
  intersectable_data.reserve(total_size * intersectable_stride);
  material_data.reserve(total_size * material_stride);

  const auto add_material = [&material_data](const Material& material) {
    material_data.emplace_back(vec4(material.albedo, 0.0));
    material_data.emplace_back(vec4(material.metallic, material.roughness, material.ao, 0.0));

    vec3 f0 = glm::mix(vec3(0.04f), material.albedo, material.metallic);
    vec3 reflectance = (f0 + (vec3(1.0f) - f0) * pow(0.5f, 5.0f)) * (1.0f - material.roughness);
    material_data.emplace_back(vec4(reflectance, 0.0));
  };

  for (const auto& [triangle, material] : triangles) {
    vec3 e1 = triangle.vertices[1] - triangle.vertices[0];
    vec3 e2 = triangle.vertices[2] - triangle.vertices[0];
    vec3 n = glm::cross(e1, e2);
    intersectable_data.emplace_back(vec4(triangle.vertices[0], e2.x));
    intersectable_data.emplace_back(vec4(n, e2.y));
    intersectable_data.emplace_back(vec4(e1, e2.z));
    add_material(material);
  }

  constexpr GLenum buffer_type = GL_SHADER_STORAGE_BUFFER;

  glBindBuffer(buffer_type, intersectables);
  glBufferStorage(buffer_type,
                  static_cast<long>(total_size * intersectable_stride * sizeof (vec4)),
                  intersectable_data.data(), 0);
  glBindBufferBase(buffer_type, 3, intersectables);

  glBindBuffer(buffer_type, materials);
  glBufferStorage(buffer_type,
                  static_cast<long>(total_size * material_stride * sizeof (vec4)),
                  material_data.data(), 0);
  glBindBufferBase(buffer_type, 4, materials);

  glBindBuffer(buffer_type, 0);

  tree = build_bvh();
  tree->finalize();
}

std::unique_ptr<BVH> IntersectableManager::build_bvh()
{
  std::vector<std::unique_ptr<Intersectable>> intrs;
  std::transform(triangles.cbegin(), triangles.cend(), std::back_inserter(intrs), [](const auto& i) {
    return std::make_unique<Triangle>(i.first);
  });

  return std::make_unique<BVH>(std::move(intrs), 3);
}

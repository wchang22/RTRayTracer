#include "light.h"

#include <glad/glad.h>
#include <iostream>

Light::Light()
{
  glGenBuffers(1, &lights);
  glGenBuffers(1, &num_lights);
}

Light::~Light()
{
  glDeleteBuffers(1, &lights);
  glDeleteBuffers(1, &num_lights);
}

void Light::add_point_light(PointLight &&light)
{
  point_lights.emplace_back(std::move(light));
}

void Light::finalize()
{
  int num_point_lights = static_cast<int>(point_lights.size());
  glBindBuffer(GL_UNIFORM_BUFFER, num_lights);
  glBufferData(GL_UNIFORM_BUFFER, sizeof (int), &num_point_lights, GL_STATIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 5, num_lights);

  constexpr unsigned int light_stride = 2;

  std::vector<vec4> light_data;
  light_data.reserve(point_lights.size() * light_stride);

  for (const auto& light : point_lights) {
    light_data.emplace_back(vec4(light.position, 0.0));
    light_data.emplace_back(vec4(light.color, 0.0));
  }

  constexpr GLenum buffer_type = GL_SHADER_STORAGE_BUFFER;

  glBindBuffer(buffer_type, lights);
  glBufferStorage(buffer_type,
                  static_cast<long>(point_lights.size() * light_stride * sizeof (vec4)),
                  light_data.data(), 0);
  glBindBufferBase(buffer_type, 6, lights);

  glBindBuffer(buffer_type, 0);
}

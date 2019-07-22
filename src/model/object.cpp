#include "object.h"

#include <numeric>

unsigned int Object::UBO = 0;
unsigned int Object::SSBO = 0;

Object::Object() : EBO(0), num_vertices(0), num_indices(0)
{
  if (UBO == 0) {
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
  }
  if (SSBO == 0) {
    glGenBuffers(1, &SSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
}

Object::Object(Object&& other)
  : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO),
    num_vertices(other.num_vertices), num_indices(other.num_indices)
{
  other.VAO = 0;
  other.VBO = 0;
  other.EBO = 0;
}

Object::~Object()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void Object::start_setup()
{
  glBindVertexArray(VAO);
}

void Object::add_vertices(const void* vertices, int num_vertices, size_t size, GLenum draw_type)
{
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, static_cast<long>(size), vertices, draw_type);
  this->num_vertices = num_vertices;
}

void Object::add_indices(const void* indices, int num_indices, size_t size, GLenum draw_type)
{
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(size), indices, draw_type);
  this->num_indices = num_indices;
}

void Object::add_vertex_attribs(std::initializer_list<int> vertex_attrib_sizes)
{
  int stride = std::accumulate(vertex_attrib_sizes.begin(), vertex_attrib_sizes.end(), 0) *
               static_cast<int>(sizeof (float));
  unsigned int pointer_num = 0;
  int offset = 0;

  for (auto size : vertex_attrib_sizes) {
    glVertexAttribPointer(pointer_num, size, GL_FLOAT, GL_FALSE,
                          stride, reinterpret_cast<void*>(offset));
    glEnableVertexAttribArray(pointer_num);
    pointer_num++;
    offset += size * static_cast<int>(sizeof (float));
  }
}

void Object::finalize_setup()
{
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::set_model_transforms(const std::vector<Transform>& transforms)
{
  std::vector<mat4> model_matrices;
  model_matrices.reserve(transforms.size());

  for (const auto& [scale, rotate, translate] : transforms) {
    mat4 model(1.0f);
    if (translate.has_value()) {
      model *= glm::translate(translate.value());
    }
    if (rotate.has_value()) {
      auto [angle, axis] = rotate.value();
      model *= glm::rotate(angle, axis);
    }
    if (scale.has_value()) {
      model *= glm::scale(scale.value());
    }

    model_matrices.emplace_back(std::move(model));
  }

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<int>(model_matrices.size() * sizeof (mat4)),
               model_matrices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Object::set_world_space_transform(mat4 perspective, mat4 view)
{
  glBindBuffer(GL_UNIFORM_BUFFER, UBO);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof (mat4), &perspective[0][0]);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof (mat4), sizeof (mat4), &view[0][0]);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Object::draw(const Shader& shader, std::initializer_list<std::string_view> flags) const
{
  draw_instanced(shader, 1, flags);
}

void Object::draw_instanced(const Shader& shader, int num_times,
                            std::initializer_list<std::string_view> flags) const
{
  shader.use();

  for (const auto& flag : flags) {
    glUniform1i(shader.get_uniform_location(flag), 1);
  }

  glBindVertexArray(VAO);

  if (EBO == 0) {
    glDrawArraysInstanced(GL_TRIANGLES, 0, num_vertices, num_times);
  } else {
    glDrawElementsInstanced(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT,
                            reinterpret_cast<void*>(0), num_times);
  }

  glBindVertexArray(0);

  for (const auto& flag : flags) {
    glUniform1i(shader.get_uniform_location(flag), 0);
  }
}

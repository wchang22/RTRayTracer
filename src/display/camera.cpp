#include "camera.h"
#include "window.h"

#include <algorithm>

#include <GLFW/glfw3.h>

Camera::Camera(vec3 position, vec3 forward, vec3 up, int width, int height, float fovy)
  : up(glm::normalize(up)),
    position(position),
    forward(glm::normalize(forward)),
    fovy(fovy),
    width(width),
    height(height)
{
  glGenBuffers(1, &UBO);

  vec2 coord_scale = get_coord_scale();
  vec2 coord_dims = get_coord_dims();

  glBindBuffer(GL_UNIFORM_BUFFER, UBO);
  glBufferData(GL_UNIFORM_BUFFER, 5 * sizeof (vec4), nullptr, GL_DYNAMIC_DRAW);
  vec4* ubo_ptr = reinterpret_cast<vec4*>(glMapNamedBufferRange(UBO, 0, sizeof (vec4),
                                                                GL_MAP_WRITE_BIT));
  ubo_ptr[0] = vec4(coord_scale, coord_dims);
  glUnmapNamedBuffer(UBO);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, UBO);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Camera::~Camera()
{
  glDeleteBuffers(1, &UBO);
}

mat3 Camera::get_coord_frame() const
{
  vec3 w = -glm::normalize(forward);
  vec3 u = glm::normalize(glm::cross(up, w));
  vec3 v = glm::cross(w, u);

  return mat3(u, v, w);
}

vec2 Camera::get_coord_scale() const
{
  vec2 half_fov(fovy * width / height / 2.0f, fovy / 2.0f);
  vec2 half_dims(width / 2.0f, height / 2.0f);
  return vec2(tan(radians(half_fov)) / half_dims);
}

vec2 Camera::get_coord_dims() const
{
  return vec2(width, height) / 2.0f;
}

void Camera::move(Direction direction) {
  switch (direction) {
    case Direction::FORWARD:
      position += forward * speed;
      break;
    case Direction::BACKWARD:
      position += -forward * speed;
      break;
    case Direction::LEFT:
      position += glm::normalize(glm::cross(up, forward)) * speed;
      break;
    case Direction::RIGHT:
      position += glm::normalize(glm::cross(forward, up)) * speed;
      break;
    case Direction::UP:
      position += up * speed;
      break;
    case Direction::DOWN:
      position += -up * speed;
      break;
  }
}

void Camera::update_frames() {
  float current_frame = static_cast<float>(glfwGetTime());
  time_delta = current_frame - last_frame;
  last_frame = current_frame;
  speed = 2.5f * time_delta;

  vec3 camera_pos = get_position();
  mat3 coord_frame = get_coord_frame();

  glBindBuffer(GL_UNIFORM_BUFFER, UBO);
  vec4* ubo_ptr = reinterpret_cast<vec4*>(
                    glMapNamedBufferRange(UBO, sizeof (vec4), 4 * sizeof (vec4),
                                          GL_MAP_WRITE_BIT));
  ubo_ptr[0] = vec4(camera_pos, 0.0);
  ubo_ptr[1] = vec4(coord_frame[0], 0.0);
  ubo_ptr[2] = vec4(coord_frame[1], 0.0);
  ubo_ptr[3] = vec4(coord_frame[2], 0.0);
  glUnmapNamedBuffer(UBO);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::update_direction(float delta_x, float delta_y) {
  yaw += delta_x;
  pitch = std::clamp(pitch + delta_y, -89.0f, 89.0f);

  forward.x = static_cast<float>(cos(static_cast<double>(glm::radians(pitch))) *
                                   cos(static_cast<double>(glm::radians(yaw))));
  forward.y = static_cast<float>(sin(static_cast<double>(glm::radians(pitch))));
  forward.z = static_cast<float>(cos(static_cast<double>(glm::radians(pitch))) *
                                   sin(static_cast<double>(glm::radians(yaw))));
  forward = glm::normalize(forward);
}

void Camera::update_direction(vec3 direction)
{
  forward = direction;
}

void Camera::update_position(vec3 position)
{
  this->position = position;
}

vec3 Camera::get_position() const {
  return position;
}

vec3 Camera::get_direction() const {
  return forward;
}

void Camera::circle()
{
  update_position(vec3(6.0f * static_cast<float>(cos(glfwGetTime() / 2.0)),
                       4.0f,
                       6.0f * static_cast<float>(sin(glfwGetTime() / 2.0))));
  update_direction(vec3(0.0f, 0.0f, 0.0f) - get_position());
}

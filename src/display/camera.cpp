#include "camera.h"
#include "window.h"

#include <algorithm>

#include <GLFW/glfw3.h>

Camera::Camera(vec3 position, vec3 forward, vec3 up)
  : up(up),
    position(position),
    forward(forward)
{
}

mat4 Camera::lookat() const {
  return glm::lookAt(position, position + forward, up);
}

mat4 Camera::perspective() const {
  return glm::perspective(glm::radians(fovy),
                          static_cast<float>(Window::width()) / Window::height(),
                          0.1f, 100.0f);
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

vec3 Camera::get_position() const {
  return position;
}

vec3 Camera::get_direction() const {
  return forward;
}

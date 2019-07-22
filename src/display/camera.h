#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera
{
public:
  Camera(vec3 position, vec3 forward, vec3 up);

  enum class Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
  };

  mat4 lookat() const;
  mat4 perspective() const;
  void move(Direction direction);
  void update_frames();
  void update_direction(float delta_x, float delta_y);
  vec3 get_position() const;
  vec3 get_direction() const;

private:
  vec3 up;
  vec3 position;
  vec3 forward;

  float speed = 0.0f;
  float time_delta = 0.0f;
  float last_frame = 0.0f;
  float pitch = 0.0f;
  float yaw = 270.0f;
  float fovy = 45.0f;
};

#endif // CAMERA_H

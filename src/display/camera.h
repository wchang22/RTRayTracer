#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera
{
public:
  Camera(vec3 position, vec3 forward, vec3 up, int width, int height, float fovy);
  ~Camera();

  enum class Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
  };

  mat3 get_coord_frame() const;
  vec2 get_coord_scale() const;
  vec2 get_coord_dims() const;
  void move(Direction direction);
  void update_frames();
  void update_direction(float delta_x, float delta_y);
  void update_direction(vec3 direction);
  void update_position(vec3 position);
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
  float fovy;

  int width, height;

  unsigned int UBO;
};

#endif // CAMERA_H

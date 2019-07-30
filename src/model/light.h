#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include <glm/glm.hpp>

using namespace glm;

class Light
{
public:
  Light();
  ~Light();

  struct PointLight {
    vec3 position;
    vec3 attenuation;
  };

  void add_point_light(PointLight&& light);
  void finalize();

private:
  unsigned int lights, num_lights;
  std::vector<PointLight> point_lights;
};

#endif // LIGHT_H

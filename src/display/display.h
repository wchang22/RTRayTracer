#ifndef DISPLAY_H
#define DISPLAY_H

#include "model/object.h"
#include "shader/shader.h"
#include "shader/image.h"
#include "display/camera.h"

#include <memory>

class Display {
public:
  Display(std::shared_ptr<Camera> camera);

  void draw() const;

private:
  std::shared_ptr<Camera> camera;
  Object rect;
  Shader rect_shader;
  Shader compute_shader;
  Image image;
};

#endif // DISPLAY_H
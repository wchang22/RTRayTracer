#ifndef WINDOW_H
#define WINDOW_H

#include "display/display.h"
#include "display/camera.h"

#include <memory>

#include <GLFW/glfw3.h>

using namespace glm;

class Window {
public:
  Window();
  ~Window();

  void main_loop();

  static int get_width();
  static int get_height();

private:
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  void key_callback();
  void mouse_callback();

  GLFWwindow* window;
  std::unique_ptr<Display> display;
  std::shared_ptr<Camera> camera;

  static int width, height;
};

#endif // WINDOW_H

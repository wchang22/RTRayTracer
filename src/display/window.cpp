#include "window.h"
#include "util/exception.h"
#include "util/data.h"

Window::Window()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const int width = Window::width();
  const int height = Window::height();
  window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);

  if (!window) {
    glfwDestroyWindow(window);
    glfwTerminate();
    throw WindowException("Failed to create GLFW Window");
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    glfwDestroyWindow(window);
    glfwTerminate();
    throw WindowException("Failed to create initialize GLAD");
  }

  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  camera = std::make_shared<Camera>(vec3(0.0f, 0.0f, 4.0f),
                                    vec3(0.0f, 0.0f, -1.0f),
                                    vec3(0.0f, 1.0f, 0.0f),
                                    width, height, 45.0f);

  try {
    display = std::make_unique<Display>(camera);
  } catch (...) {
    glfwDestroyWindow(window);
    std::rethrow_exception(std::current_exception());
  }
}

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::main_loop() {
  glEnable(GL_DEPTH_TEST);

  try {
    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      display->draw();

      glfwSwapBuffers(window);
      glfwPollEvents();
      key_callback();
      mouse_callback();
    }
  } catch (...) {
    glfwDestroyWindow(window);
    std::rethrow_exception(std::current_exception());
  }
}

int Window::width()
{
  return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
}

int Window::height()
{
  return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  (void) window;
  glViewport(0, 0, width, height);
}

void Window::key_callback() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera->move(Camera::Direction::FORWARD);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera->move(Camera::Direction::BACKWARD);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera->move(Camera::Direction::LEFT);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera->move(Camera::Direction::RIGHT);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera->move(Camera::Direction::UP);
  }
  if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
    camera->move(Camera::Direction::DOWN);
  }
}

void Window::mouse_callback() {
  constexpr float MOUSE_SENSITIVITY = 0.05f;

  static double prev_x, prev_y;
  double x, y;
  glfwGetCursorPos(window, &x, &y);

  if (static bool first_time = true; first_time) {
    prev_x = x;
    prev_y = y;
    first_time = false;
  }

  float delta_x = static_cast<float>(x - prev_x);
  float delta_y = static_cast<float>(prev_y - y);
  prev_x = x;
  prev_y = y;

  delta_x *= MOUSE_SENSITIVITY;
  delta_y *= MOUSE_SENSITIVITY;

  camera->update_direction(delta_x, delta_y);
}
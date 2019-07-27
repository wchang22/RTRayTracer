#include "display.h"
#include "util/data.h"
#include "display/window.h"
#include "util/profiling/profiling.h"

Display::Display(std::shared_ptr<Camera> camera)
  : camera(camera),
    rect_shader("../../shaders/object/rect.vert", "../../shaders/object/rect.frag"),
    compute_shader("../../shaders/compute/raytrace.comp",
                   static_cast<unsigned int>(Window::get_width()),
                   static_cast<unsigned int>(Window::get_height()), 1),
    image(Window::get_width(), Window::get_height())
{
  rect.start_setup();
  rect.add_vertices(QUAD_VERTICES, 6, sizeof (QUAD_VERTICES));
  rect.add_vertex_attribs({ 2, 2 });
  rect.finalize_setup();
}

void Display::draw() const
{
  PROFILE_SCOPE("Draw")

  PROFILE_SECTION_START("Update Camera")
  camera->update_position(vec3(5.0f * static_cast<float>(cos(glfwGetTime() / 2.0)),
                               3.0f,
                               5.0f * static_cast<float>(sin(glfwGetTime() / 2.0))));
  camera->update_direction(vec3(0.0f, -0.5f, 0.0f) - camera->get_position());
  camera->update_frames();
  PROFILE_SECTION_END()

  PROFILE_SECTION_START("Compute raytracing")
  compute_shader.use();
  compute_shader.dispatch_compute();
  PROFILE_SECTION_END()

  PROFILE_SECTION_START("Draw to screen")
  image.use(rect_shader);
  rect.draw(rect_shader);
  PROFILE_SECTION_END()
}

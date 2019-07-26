#include "display.h"
#include "util/data.h"
#include "display/window.h"

Display::Display(std::shared_ptr<Camera> camera)
  : camera(camera),
    rect_shader("../../shaders/object/rect.vert", "../../shaders/object/rect.frag"),
    compute_shader("../../shaders/compute/compute.comp",
                   static_cast<unsigned int>(Window::width()),
                   static_cast<unsigned int>(Window::height()), 1),
    image(Window::width(), Window::height())
{
  rect.start_setup();
  rect.add_vertices(QUAD_VERTICES, 6, sizeof (QUAD_VERTICES));
  rect.add_vertex_attribs({ 2, 2 });
  rect.finalize_setup();
}

void Display::draw() const
{
  camera->update_frames();

  compute_shader.use();
  compute_shader.dispatch_compute();

  image.use(rect_shader);
  rect.draw(rect_shader);
}

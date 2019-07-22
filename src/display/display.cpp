#include "display.h"
#include "util/data.h"

Display::Display(std::shared_ptr<Camera> camera)
  : camera(camera),
    rect_shader("../../shaders/object/rect.vert", "../../shaders/object/rect.frag")
{
  rect.start_setup();
  rect.add_vertices(QUAD_VERTICES, 6, sizeof (QUAD_VERTICES));
  rect.add_vertex_attribs({ 2, 2 });
  rect.finalize_setup();
}

void Display::draw() const
{
  camera->update_frames();

  rect.draw(rect_shader);
}

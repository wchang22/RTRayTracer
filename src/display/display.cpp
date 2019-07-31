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
  image.add_image(GL_RGBA8, false, true);

  rect.start_setup();
  rect.add_vertices(QUAD_VERTICES, 6, sizeof (QUAD_VERTICES));
  rect.add_vertex_attribs({ 2, 2 });
  rect.finalize_setup();

  intersectable.add_sphere({ vec3(0.0f, 0.8f, 1.0f), 0.8f },
                           { vec3(0.01f, 0.1f, 0.01f), vec3(0.2f, 0.3f, 0.2f), vec3(0.4f, 0.7f, 0.4f), 32 });
  intersectable.add_sphere({ vec3(3.0f, 1.0f, 0.0f), 1.0f },
                           { vec3(0.1f, 0.01f, 0.01f), vec3(0.3f, 0.2f, 0.2f), vec3(0.7f, 0.4f, 0.4f), 32 });
  intersectable.add_sphere({ vec3(-3.0f, 0.5f, -0.5f), 0.5f },
                           { vec3(0.01f, 0.01f, 0.1f), vec3(0.2f, 0.2f, 0.3f), vec3(0.4f, 0.4f, 0.7f), 32 });
  intersectable.add_sphere({ vec3(1.0f, 0.75f, -2.0f), 0.75f },
                           { vec3(0.1f, 0.1f, 0.01f), vec3(0.3f, 0.3f, 0.2f), vec3(0.7f, 0.7f, 0.4f), 32 });
  intersectable.add_sphere({ vec3(-2.0f, 0.6f, -3.0f), 0.6f },
                           { vec3(0.0f), vec3(0.0f), vec3(0.8f), 255 });

  intersectable.add_triangle({ vec3(8.0f, 0.0f, 8.0f), vec3(8.0f, 0.0f, -8.0f), vec3(-8.0f, 0.0f, -8.0f) },
                             { vec3(0.01f), vec3(0.2f), vec3(0.2f), 32 });
  intersectable.add_triangle({ vec3(8.0f, 0.0f, 8.0f), vec3(-8.0f, 0.0f, -8.0f), vec3(-8.0f, 0.0f, 8.0f) },
                             { vec3(0.01f), vec3(0.2f), vec3(0.2f), 32 });
  intersectable.finalize();

  light.add_point_light({ vec3(1.5, 2.5, -2.0), vec3(1.0, 0.045, 0.016) });
  light.add_point_light({ vec3(-1.5, 2.5, -2.0), vec3(1.0, 0.045, 0.016) });
  light.add_point_light({ vec3(0.0, 2.5, 2.0), vec3(1.0, 0.045, 0.016) });
  light.add_point_light({ vec3(-3.0, 10.0, 1.0), vec3(1.0, 0.09, 0.032) });
  light.add_point_light({ vec3(4.0, 10.0, -4.0), vec3(1.0, 0.09, 0.032) });
  light.finalize();
}

void Display::draw() const
{
  PROFILE_SCOPE("Draw");

  PROFILE_SECTION_START("Update Camera");
  camera->update_frames();
  PROFILE_SECTION_END();

  PROFILE_SECTION_START("Compute raytracing");
  compute_shader.use();
  compute_shader.dispatch_compute();
  PROFILE_SECTION_END();

  PROFILE_SECTION_START("Draw to screen");
  image.use(rect_shader);
  rect.draw(rect_shader);
  PROFILE_SECTION_END();
}

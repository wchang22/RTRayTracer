#include "image.h"

#include "util/exception.h"

Image::Image(int width, int height)
  : width(width), height(height)
{

}

Image::~Image()
{
  glDeleteTextures(static_cast<int>(textures.size()), textures.data());
}

void Image::add_image(GLenum image_format, bool read, bool write)
{
  GLenum read_write_policy;
  if (read && write) {
    read_write_policy = GL_READ_WRITE;
  } else if (read) {
    read_write_policy = GL_READ_ONLY;
  } else if (write) {
    read_write_policy = GL_WRITE_ONLY;
  } else {
    throw ImageException("Image must be read only, write only, or read-write");
  }

  textures.emplace_back(0);
  glGenTextures(1, &textures.back());
  glBindTexture(GL_TEXTURE_2D, textures.back());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexStorage2D(GL_TEXTURE_2D, 1, image_format, width, height);
  glBindImageTexture(static_cast<unsigned int>(textures.size() - 1),
                     textures.back(), 0, GL_FALSE, 0, read_write_policy, image_format);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::use(const Shader& shader) const
{
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  shader.use();
  for (unsigned int i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glUniform1i(shader.get_uniform_location("texture_image" + std::to_string(i + 1)),
                static_cast<int>(i));
    glBindTexture(GL_TEXTURE_2D, textures[i]);
  }
}

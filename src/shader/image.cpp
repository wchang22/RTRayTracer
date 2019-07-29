#include "image.h"

#include <glad/glad.h>
#include <iostream>

Image::Image(int width, int height)
{
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
  glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Image::~Image()
{
  glDeleteTextures(1, &texture);
}

void Image::use(const Shader& shader) const
{
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);

  shader.use();
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(shader.get_uniform_location("texture_image"), 0);
  glBindTexture(GL_TEXTURE_2D, texture);
}

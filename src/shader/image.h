#ifndef IMAGE_H
#define IMAGE_H

#include "shader/shader.h"

#include <glad/glad.h>
#include <vector>

class Image
{
public:
  Image(int width, int height);
  ~Image();

  void add_image(GLenum image_format, bool read = true, bool write = true);
  void use(const Shader& shader) const;

private:
  std::vector<unsigned int> textures;
  int width, height;
};

#endif // IMAGE_H

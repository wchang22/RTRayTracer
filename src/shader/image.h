#ifndef IMAGE_H
#define IMAGE_H

#include "shader/shader.h"

class Image
{
public:
  Image(int width, int height);
  ~Image();

  void use() const;

private:
  unsigned int texture;
};

#endif // IMAGE_H

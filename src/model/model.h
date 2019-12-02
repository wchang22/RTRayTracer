#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "shader/shader.h"
#include "intersectable/intersectable_manager.h"

typedef glm::mat4 mat4;
typedef glm::vec3 vec3;

class Model
{
public:
  Model(const char* path, IntersectableManager& intersectables);

private:
  void load_model(std::string_view path);
  void process_node(aiNode* node, const aiScene* scene);
  void process_mesh(aiMesh* mesh);

  IntersectableManager& intersectables;
  std::string directory;
};

#endif // MODEL_H

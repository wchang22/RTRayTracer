#ifndef KDTREE_H
#define KDTREE_H

#include "model/intersectable/aabb.h"

#include <vector>
#include <memory>

class KDTree
{
public:
  KDTree(std::vector<std::unique_ptr<Intersectable>>&& intersectables, int max_depth);

private:
  struct KDNode {
    KDNode(const vec3& center, const vec3& lengths) : aabb(center, lengths) {}

    AABB aabb;
    std::unique_ptr<KDNode> left;
    std::unique_ptr<KDNode> right;
    std::vector<unsigned int> intersectables;
  };

  void build_empty_tree(int depth, std::unique_ptr<KDNode>& parent_node, std::vector<vec3>& centers,
                        unsigned int start, unsigned int end);
  bool insert_intersectable(std::unique_ptr<KDNode>& node, unsigned int intersectable);
  void prune(std::unique_ptr<KDNode>& node);

  std::vector<std::unique_ptr<Intersectable>> intersectables;
  std::unique_ptr<KDNode> tree;
  int max_depth;
};

#endif // KDTREE_H

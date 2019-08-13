#ifndef KDTREE_H
#define KDTREE_H

#include "model/intersectable/aabb.h"

#include <vector>
#include <memory>

class KDTree
{
public:
  KDTree(std::vector<std::unique_ptr<Intersectable>>&& intersectables, int max_depth);
  ~KDTree();

  void finalize();

private:
  struct KDNode {
    KDNode(const vec3& center, const vec3& lengths) : aabb(center, lengths) {}

    AABB aabb;
    std::unique_ptr<KDNode> left;
    std::unique_ptr<KDNode> right;
    std::vector<unsigned int> intersectables;
  };

  struct FlatKDNode {
    FlatKDNode(const vec4& bound1, const vec4& bound2, int left, int right,
               int intersectables_offset, int num_intersectables)
      : bound1(bound1), bound2(bound2), left(left), right(right),
        intersectables_offset(intersectables_offset), num_intersectables(num_intersectables) {}

    vec4 bound1;
    vec4 bound2;
    int left;
    int right;
    int intersectables_offset;
    int num_intersectables;
  };

  void build_empty_tree(int depth, std::unique_ptr<KDNode>& parent_node,
                        std::vector<vec3>& centers, unsigned int start, unsigned int end);
  bool insert_intersectable(std::unique_ptr<KDNode>& node, unsigned int intersectable);
  void prune(std::unique_ptr<KDNode>& node);

  int build_flat_kd_tree(std::vector<FlatKDNode>& flat_kd_tree,
                         std::vector<std::pair<int, Intersectable::Type>>& kd_contains,
                         std::unique_ptr<KDNode>& node);

  std::vector<std::unique_ptr<Intersectable>> intersectables;
  std::unique_ptr<KDNode> tree;
  int max_depth;
  unsigned int kd_tree_buffer, kd_contains_buffer;
};

#endif // KDTREE_H

#ifndef BVH_H
#define BVH_H

#include "model/intersectable/aabb.h"

#include <vector>
#include <memory>

class BVH
{
public:
  BVH(std::vector<std::unique_ptr<Intersectable>>&& intersectables, int max_depth);
  ~BVH();

  void finalize();

private:
  struct BVHNode {
    BVHNode(const vec3& center, const vec3& lengths) : aabb(center, lengths) {}

    AABB aabb;
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::vector<unsigned int> intersectables;
  };

  struct FlatBVHNode {
    FlatBVHNode(const vec4& bound1, const vec4& bound2, int left, int right,
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

  void build_empty_bvh(int depth, std::unique_ptr<BVHNode>& parent_node,
                        std::vector<vec3>& centers, unsigned int start, unsigned int end);
  bool insert_intersectable(std::unique_ptr<BVHNode>& node, unsigned int intersectable);
  void prune(std::unique_ptr<BVHNode>& node);

  int build_flat_bvh(std::vector<FlatBVHNode>& flat_bvh,
                         std::vector<std::pair<int, Intersectable::Type>>& bvh_contains,
                         std::unique_ptr<BVHNode>& node);

  std::vector<std::unique_ptr<Intersectable>> intersectables;
  std::unique_ptr<BVHNode> tree;
  int max_depth;
  unsigned int bvh_buffer, bvh_contains_buffer;
};

#endif // BVH_H

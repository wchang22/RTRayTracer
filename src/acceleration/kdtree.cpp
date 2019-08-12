#include "kdtree.h"

#include <algorithm>

KDTree::KDTree(std::vector<std::unique_ptr<Intersectable>>&& intersectables, int max_depth)
  : intersectables(std::move(intersectables)), max_depth(max_depth)
{
  float inf = std::numeric_limits<float>::infinity();
  vec2 largest_bounds[3] = {{inf, -inf}, {inf, -inf}, {inf, -inf}};

  // Find bounds of AABB that encompass all intersectables
  for (const auto& intrs : this->intersectables) {
    for (int axis = 0; axis < 3; axis++) {
      vec2 bound = intrs->get_bounds(static_cast<Intersectable::Axis>(axis));
      largest_bounds[axis][0] = std::min(largest_bounds[axis][0], bound[0]);
      largest_bounds[axis][1] = std::max(largest_bounds[axis][1], bound[1]);
    }
  }

  // Build said AABB
  vec3 center;
  vec3 lengths;
  for (int axis = 0; axis < 3; axis++) {
    center[axis] = (largest_bounds[axis][0] + largest_bounds[axis][1]) / 2.0f;
    lengths[axis] = largest_bounds[axis][1] - largest_bounds[axis][0];
  }

  // Insert root into tree
  tree = std::make_unique<KDNode>(center, lengths);

  std::vector<vec3> centers;

  // Get list of intersectable centers
  std::transform(this->intersectables.cbegin(), this->intersectables.cend(),
    std::back_inserter(centers), [](const auto& intrs) {
      return intrs->get_center();
    }
  );

  // Build tree
  build_empty_tree(0, tree, centers, 0, static_cast<unsigned int>(this->intersectables.size()));

  // Insert intersectables into tree
  for (unsigned int i = 0; i < this->intersectables.size(); i++) {
    insert_intersectable(tree, i);
  }

  // Prune tree to get rid of extra nodes
  prune(tree);
}

void KDTree::build_empty_tree(int depth, std::unique_ptr<KDNode>& parent_node,
                              std::vector<vec3>& centers, unsigned int start, unsigned int end)
{
  if (depth >= max_depth) {
    return;
  }

  unsigned int num_intersectables = end - start;
  if (num_intersectables <= 0) {
    return;
  }

  int axis = depth % 3;
  unsigned int median = start + num_intersectables / 2;

  // Get median
  std::nth_element(centers.begin() + start, centers.begin() + median, centers.begin() + end,
    [axis](const auto& a, const auto& b) {
      return a[axis] < b[axis];
    }
  );

  float median_center = centers[median][axis];

  // Build left and right child nodes
  vec2 parent_bounds = parent_node->aabb.get_bounds(static_cast<Intersectable::Axis>(axis));

  vec3 left_aabb_center = parent_node->aabb.center;
  vec3 right_aabb_center = parent_node->aabb.center;
  left_aabb_center[axis] = (parent_bounds[0] + median_center) / 2.0f;
  right_aabb_center[axis] = (parent_bounds[1] + median_center) / 2.0f;

  vec3 left_aabb_lengths = parent_node->aabb.lengths;
  vec3 right_aabb_lengths = parent_node->aabb.lengths;
  left_aabb_lengths[axis] = median_center - parent_bounds[0];
  right_aabb_lengths[axis] = parent_bounds[1] - median_center;

  parent_node->left = std::make_unique<KDNode>(left_aabb_center, left_aabb_lengths);
  parent_node->right = std::make_unique<KDNode>(right_aabb_center, right_aabb_lengths);

  // Recursively build tree
  build_empty_tree(depth + 1, parent_node->left, centers, start, median);
  build_empty_tree(depth + 1, parent_node->right, centers, median, end);
}

bool KDTree::insert_intersectable(std::unique_ptr<KDNode>& node, unsigned int intersectable)
{
  if (!node || !intersectables[intersectable]->is_in_aabb(node->aabb)) {
    return false;
  }

  if (insert_intersectable(node->left, intersectable)) {
    return true;
  }

  if (insert_intersectable(node->right, intersectable)) {
    return true;
  }

  node->intersectables.emplace_back(intersectable);
  return true;
}

void KDTree::prune(std::unique_ptr<KDTree::KDNode>& node)
{
  if (!node) {
    return;
  }

  prune(node->left);
  prune(node->right);

  // If left has no intersectables and no children, prune
  if (node->left && node->left->intersectables.empty() &&
      !node->left->left && !node->left->right) {
    node->left = nullptr;
  }

  // If right has no intersectables and no children, prune
  if (node->right && node->right->intersectables.empty() &&
      !node->right->left && !node->right->right) {
    node->right = nullptr;
  }
}
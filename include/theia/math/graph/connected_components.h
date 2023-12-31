// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_MATH_GRAPH_CONNECTED_COMPONENTS_H_
#define THEIA_MATH_GRAPH_CONNECTED_COMPONENTS_H_

#include <glog/logging.h>

#include <limits>
#include <unordered_map>
#include <unordered_set>

#include "theia/util/map_util.h"

namespace theia {

// A connected components algorithm based on the union-find structure. Connected
// components from a graph are needed for estimating poses from a view graph and
// for generating tracks from image correspondences.
//
// This particular implementation can utilize an upper limit on the size of a
// connected component. This is useful when generating tracks in SfM since large
// tracks are increasingly likely to have outliers.
//
// NOTE: The template parameter T must be a type compatible with
// numeric_limits<T> e.g. int, uint16_t, etc.
template <typename T>
class ConnectedComponents {
 public:
  // The Root struct is used to store the connected component that each node is
  // a part of. Each node is mapped to a Root and all nodes that map to a root
  // with the same ID are part of the same connected component.
  struct Root {
    Root(const T& id, const int size) : id(id), size(size) {}
    T id;
    int size;
  };

  ConnectedComponents()
      : max_connected_component_size_(std::numeric_limits<T>::max()) {}

  // Specify the maximum connected component size.
  explicit ConnectedComponents(const int max_size)
      : max_connected_component_size_(max_size) {
    CHECK_GT(max_connected_component_size_, 0);
  }

  // Adds an edge connecting the two nodes to the connected component graph. The
  // edge is inserted as a new connected component if the edge is not present in
  // the graph. The edge is added to the current connected component if at least
  // one of the nodes already exists in the graph, and connected components are
  // merged if appropriate. If adding the edge to the graph creates a connected
  // component larger than the maximum allowable size then we simply create a
  // new connected component.
  void AddEdge(const T& node1, const T& node2) {
    Root* root1 = FindOrInsert(node1);
    Root* root2 = FindOrInsert(node2);

    // If the nodes are already part of the same connected component then do
    // nothing. If merging the connected components will create a connected
    // component larger than the max size then do nothing.
    if (root1->id == root2->id ||
        root1->size + root2->size > max_connected_component_size_) {
      return;
    }

    // Union the two connected components. Balance the tree better by attaching
    // the smaller tree to the larger one.
    if (root1->size < root2->size) {
      root2->size += root1->size;
      *root1 = *root2;
    } else {
      root1->size += root2->size;
      *root2 = *root1;
    }
  }

  // Computes the connected components and returns the disjointed sets.
  void Extract(
      std::unordered_map<T, std::unordered_set<T> >* connected_components) {
    CHECK_NOTNULL(connected_components)->clear();

    for (const auto& node : disjoint_set_) {
      const Root* root = FindRoot(node.first);
      (*connected_components)[root->id].insert(node.first);
    }
  }

 private:
  // Attempts to find the root of the tree, or otherwise inserts the node.
  Root* FindOrInsert(const T& node) {
    const Root* parent = FindOrNull(disjoint_set_, node);
    // If we cannot find the node in the disjoint set list, insert it.
    if (parent == nullptr) {
      InsertOrDie(&disjoint_set_, node, Root(node, 1));
      return FindOrNull(disjoint_set_, node);
    }

    return FindRoot(node);
  }

  // Perform a recursive search to find the root of the node. We flatten the
  // tree structure as we proceed so that finding the root is always a few
  // (hopefully one) steps away.
  Root* FindRoot(const T& node) {
    Root* parent = CHECK_NOTNULL(FindOrNull(disjoint_set_, node));

    // If this node is a root, return the node itself.
    if (node == parent->id) {
      return parent;
    }

    // Otherwise, recusively search for the root.
    Root* root = FindRoot(parent->id);
    *parent = *root;
    return root;
  }

  uint64_t max_connected_component_size_;

  // Each node is mapped to a Root node. If the node is equal to the root id
  // then the node is a root and the size of the root is the size of the
  // connected component.
  std::unordered_map<T, Root> disjoint_set_;
};

}  // namespace theia

#endif  // THEIA_MATH_GRAPH_CONNECTED_COMPONENTS_H_

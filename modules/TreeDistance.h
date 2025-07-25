#include "Tree.h"
#include <vector>

template <Tree T>
class TreeDistance {
public:
	TreeDistance(T& tree):
		distances(tree.size()) {
		init_distances(tree, 0);
	}
private:
	std::vector<typename T::edge_t> distances;

	void init_distances(T& tree, T::index_t parent) {
		for (auto const& child: tree.children(parent)) {
			distances[child] = distances[parent] + child.edge();
			init_distances(tree, child);
		}
	}

public:
	uint operator[](T::index_t index) {
		return distances[index];
	}
};

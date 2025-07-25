#pragma once

#include "Tree.h"
#include "TreeDistance.h"
#include "SparseTable.h"
#include <vector>

template <Tree T>
class LCA {
public:
	LCA(T& tree):
		tree(tree),
		distances(tree.size()),
		parent_table(tree.size(), tree.size() - 1,
				[&tree](T::index_t idx) {return tree.parent(idx);}) {
		init_distances(tree.root());
	}
private:
	T& tree;
	std::vector<typename T::index_t> distances;
	SparseTable parent_table;

	void init_distances(T::index_t parent) {
		for (auto const& child: tree.children(parent)) {
			distances[child] = distances[parent] + 1;
			init_distances(child);
		}
	}

public:
	T::index_t lca(T::index_t a, T::index_t b) {
		if (distances[a] > distances[b])
			a = parent_table.after(a, distances[a] - distances[b]);
		else
		 	b = parent_table.after(b, distances[b] - distances[a]);

		return parent_table.follow_until_same(a, b);
	}
};

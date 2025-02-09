#include "Graph.h"

template <typename G> requires Graph<G>
void Treeify(G& graph, typename G::index_t parent, typename G::index_t before_parent) {
	for (auto& it: graph.children()) {
	}
}

template <typename G> requires Graph<G>
void Treeify(G& graph, typename G::index_t root) {
	Treeify(graph, root, root);
}

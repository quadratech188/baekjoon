#include "../modules/hackable_private.h"
#include "../modules/Graph.h"
#include <limits>
#include <stack>
#include <vector>

template <Graph G, typename B>
class SSC {
public:
	SSC(G& graph, B& bins):
		graph(graph), bins(bins),
		indices(graph.size(), std::numeric_limits<uint>::max()),
		lowlinks(graph.size(), std::numeric_limits<uint>::max()),
		on_stack(graph.size(), false),
		index(0),
		S() {}

	void operator()() {
		for (typename G::index_t i = 0; i < graph.size(); i++) {
			if (indices[i] == std::numeric_limits<uint>::max())
				strongconnect(i);
		}
	}

hackable_private:
	// https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
	G& graph;
	B& bins;

	G::template storage_t<uint> indices, lowlinks;
	G::template storage_t<bool> on_stack;

	uint index;
	std::stack<typename G::index_t> S;

	void strongconnect(G::index_t v) {
		indices[v] = index;
		lowlinks[v] = index;
		index ++;
		S.push(v);
		on_stack[v] = true;

		for (auto w: graph.children(v)) {
			if (indices[w] ==std::numeric_limits<uint>::max()) {
				strongconnect(w);
				lowlinks[v] = std::min(lowlinks[v], lowlinks[w]);
			}
			else if (on_stack[w]) {
				lowlinks[v] = std::min(lowlinks[v], indices[w]);
			}
		}

		if (indices[v] == lowlinks[v]) {
			bins.new_bin();

			typename G::index_t w;
			do {
				w = S.top();
				S.pop();
				on_stack[w] = false;
				bins.insert(w);
			} while (w != v);
		}
	}
};

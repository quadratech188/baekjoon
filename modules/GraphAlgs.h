#include <vector>

namespace GraphAlgs {
	template <typename Graph>
	std::vector<int> inDegree(Graph&& graph) {
		std::vector<int> result(graph.size());

		for (int parent = 0; parent < graph.size(); parent++) {
			for (int child: graph.children(parent)) {
				result[child] ++;
			}
		}

		return result;
	}

	template <typename Graph>
	std::vector<int> outDegree(Graph&& graph) {
		std::vector<int> result(graph.size());

		for (int parent = 0; parent < graph.size(); parent++) {
			for (int child: graph.children(parent)) {
				result[parent] ++;
			}
		}

		return result;
	}
}

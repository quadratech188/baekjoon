#include <concepts>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <queue>
#include <ranges>
#include <utility>
#include <variant>
#include <vector>

template <typename Vertex, typename Edge>
class ListGraph {
public:
	using index_t = int;
	using vertex_t = Vertex;
	using edge_t = Edge;

private:
	std::vector<Vertex> data;
	std::vector<std::vector<std::pair<index_t, edge_t>>> connections;
	int _size;

public:

	ListGraph(int size, const Vertex& defaultV = Vertex()): data(size, defaultV), connections(size), _size(size) {}
	ListGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(data.size()),
		_size(data.size()) {}

	index_t add(Vertex data) {
		data.push_back(data);
		connections.emplace_back();
		_size += 1;
		return _size - 1;
	}

	void reserve(int size) {
		data.reserve(size);
		connections.reserve(size);
	}

	void connect(index_t parent, index_t child, edge_t edge = edge_t()) {
		connections[parent].push_back(std::make_pair(child, edge));
	}

	size_t size() {
		return _size;
	}

	vertex_t& operator[](index_t index) {
		return data[index];
	}

	class child {
	public:
		child(ListGraph* graph, index_t parent, int list_index):
			graph(graph), parent(parent), list_index(list_index) {}

		child():
			graph(nullptr), list_index(0) {}

		index_t index() {
			return graph->connections[parent][list_index].first;
		}
		edge_t& edge() {
			return graph->connections[parent][list_index].second;
		}
		vertex_t& value() {
			return graph->data[index()];
		}

		operator index_t() {
			return index();
		}

	private:
		ListGraph* graph;
		index_t parent;
		int list_index;
	};

	auto children(index_t parent) {
		return std::ranges::iota_view(static_cast<size_t>(0), connections[parent].size())
			| std::views::transform([this, parent](index_t index) {
					return child(this, parent, index);
					});
	}
};

struct None {};

template <typename G>
concept Graph = requires(G graph, typename G::index_t u, typename G::vertex_t v, typename G::edge_t e) {
	typename G::index_t;
	typename G::vertex_t;
	typename G::edge_t;

	{graph.size()} -> std::convertible_to<std::size_t>;
	{graph[u]} -> std::convertible_to<typename G::vertex_t&>;
	{graph.connect(u, u, e)};

	{graph.children(u)} -> std::ranges::range;

	requires requires (decltype(*std::begin(graph.children(u))) child) {
		{child.index()} -> std::convertible_to<typename G::index_t>;
		{child.edge()} -> std::convertible_to<typename G::edge_t&>;
		{child.value()} -> std::convertible_to<typename G::vertex_t&>;
	};
};

namespace GraphAlgs {
	template <Graph G>
	std::vector<int> inDegree(G& graph) {
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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

void loop() {
	FastIO();
	int n, k;
	std::cin >> n >> k;

	ListGraph<int, std::monostate> graph(n);

	std::vector<int> times(n);

	for (int i = 0; i < n; i++) {
		std::cin >> times[i];
	}

	for (int i = 0; i < k; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
	}

	std::vector<int> dependencies = GraphAlgs::inDegree(graph);

	std::queue<std::pair<int, int>> queue;

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.emplace(i, times[i]);
			graph[i] = times[i];
		}
	}

	while (!queue.empty()) {
		int parent = queue.front().first;
		int time = queue.front().second;
		queue.pop();

		for (int child: graph.children(parent)) {
			dependencies[child] --;
			graph[child] = std::max(graph[child], time + times[child]);
			if (dependencies[child] == 0) {
				queue.emplace(child, graph[child]);
			}
		}
	}

	int w;
	std::cin >> w;
	std::cout << graph[w - 1] << '\n';
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++)
		loop();
}

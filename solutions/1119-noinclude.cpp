#include <cstddef>
#include <iostream>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>
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
int dfs(G graph,int parent, std::vector<bool>& visited) {
	visited[parent] = true;
	int result = 1;
	for (auto child: graph.children(parent)) {
		if (visited[child]) continue;
		result += dfs(graph, child, visited);
	}
	return result;
}

int main() {
	int n;
	std::cin >> n;
	if (n == 1) {
		std::cout << 0;
		return 0;
	}

	ListGraph<None, None> graph(n);

	int count = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			char info;
			std::cin >> info;
			if (info == 'Y') {
				graph.connect(i, j);
				count++;
			}
		}
	}

	count /= 2;

	std::vector<bool> visited(n);
	std::vector<int> s;
	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			visited[i] = true;
			s.push_back(dfs(graph, i, visited));
		}
	}

	int sum = 0;
	for (int a: s) {
		sum += a - 1;
		if (a == 1) {
			std::cout << -1;
			return 0;
		}
	}

	if (s.size() - 1 <= count - sum)
		std::cout << s.size() - 1;
	else
	 	std::cout << -1;
}

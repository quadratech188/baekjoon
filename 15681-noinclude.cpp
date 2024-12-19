#include <cstdio>
#include <functional>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

template <typename Vertex, typename Edge>
class ListGraph {
	std::vector<Vertex> data;
	std::vector<std::vector<std::pair<int, Edge>>> connections;
	int _size;

public:
	ListGraph(int size): data(size), connections(size), _size(size) {}
	ListGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(data.size()),
		_size(data.size()) {}

	int add(Vertex data) {
		data.push_back(data);
		connections.emplace_back();
		_size += 1;
		return _size - 1;
	}

	void reserve(int size) {
		data.reserve(size);
		connections.reserve(size);
	}

	void connect(int parent, int child, Edge edge = Edge()) {
		connections[parent].push_back(std::make_pair(child, edge));
	}

	template <typename Callable>
	void forEachChild(int parent, Callable&& func) {
		for (std::pair<int, Edge>& child: connections[parent]) {
			func(child.first, data[child.first], child.second);
		}
	}

	size_t size() {
		return _size;
	}

	Vertex& operator[](size_t index) {
		return data[index];
	}
};

struct None {};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

int dfs(std::vector<int>& solution, ListGraph<None, None>& graph, int parent, int root) {
	graph.forEachChild(root, [&solution, parent, root, &graph](int child, None& value, None& edge) {
			if (child == parent) return;
			solution[root] += dfs(solution, graph, root, child);
			});
	return solution[root];
}

int main() {
	FastIO();
	int n, r, q;
	std::cin >> n >> r >> q;
	ListGraph<None, None> graph(n);

	for (int i = 0; i < n - 1; i++) {
		int u, v;
		std::cin >> u >> v;
		graph.connect(u - 1, v - 1);
		graph.connect(v - 1, u - 1);
	}

	std::vector<int> solution(n, 1);
	dfs(solution, graph, r - 1, r - 1);

	for (int i = 0; i < q; i++) {
		int u;
		std::cin >> u;
		std::cout << solution[u - 1] << '\n';
	}
}

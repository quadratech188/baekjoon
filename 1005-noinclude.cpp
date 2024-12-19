#include <cstdio>
#include <iostream>
#include <queue>
#include <stack>
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

	size_t size() {
		return _size;
	}

	class Children {
	public:
		Children(ListGraph& graph, int parent): graph(graph), parent(parent) {}

		class iterator {
		public:
			iterator(ListGraph& graph, const int parent, int index = 0): graph(graph), parent(parent), _index(index) {}

			iterator& operator*() {
				return *this;
			}

			int index() const {
				return graph.connections[parent][_index].first;
			}

			Edge& edge() {
				return graph.connections[parent][_index].second;
			}

			operator int() {
				return index();
			}

			iterator& operator++() {
				_index++;
				return *this;
			}

			bool operator!=(const iterator& other) const {
				return _index != other._index;
			}

		private:
			ListGraph& graph;
			const int parent;
			int _index;
		};

		iterator begin() {
			return iterator(graph, parent);
		}

		iterator end() {
			return iterator(graph, parent, graph.connections[parent].size());
		}

	private:
		ListGraph& graph;
		int parent;
	};

	Children children(int parent) {
		return Children(*this, parent);
	}

	Vertex& operator[](size_t index) {
		return data[index];
	}
};

struct None {};

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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

void loop() {
	FastIO();
	int n, k;
	std::cin >> n >> k;

	ListGraph<int, None> graph(n);

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

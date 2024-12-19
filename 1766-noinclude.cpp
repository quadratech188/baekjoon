#include <cstdio>
#include <functional>
#include <iostream>
#include <queue>
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

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<None, None> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
	}

	std::vector<int> dependencies(n);

	for (int i = 0; i < n; i++) {
		graph.forEachChild(i, [&dependencies](int child, None& vertex, None& edge) {
				dependencies[child] ++;
				});
	}

	std::priority_queue<int> queue;

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.push(- i);
		}
	}

	while (!queue.empty()) {
		int parent = - queue.top();
		queue.pop();

		std::cout << parent + 1 << ' ';

		graph.forEachChild(parent, [&dependencies, &queue](int child, None& vertex, None& edge) {
				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(- child);
				});
	}
}

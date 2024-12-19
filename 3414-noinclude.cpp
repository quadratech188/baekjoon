#include <cstdio>
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

namespace Input {
	template<typename T>
	inline std::vector<T> inlineToVec(size_t n) {
		std::vector<T> result;
		result.reserve(n);
		for (int i = 0; i < n; i++) {
			T value;
			std::cin >> value;
			result.push_back(value);
		}
		return result;
	}
}

template <typename Vertex>
class ListGraph {
	std::vector<Vertex> data;
	std::vector<std::set<int>> connections;
	int _size;

public:
	ListGraph(int size): data(size), connections(size), _size(size) {}
	ListGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(data.size()),
		_size(data.size()) {}

	void connect(int parent, int child) {
		connections[parent].insert(child);
	}

	void forEachChild(int parent, std::function<void(int, Vertex&)> func) {
		for (int child: connections[parent]) {
			func(child, data[child]);
		}
	}

	size_t size() {
		return _size;
	}

	Vertex& operator[](size_t index) {
		return data[index];
	}
};

namespace GraphAlgs {
	template <typename Graph>
	std::vector<int> dependencies(Graph&& graph) {
		std::vector<int> result(graph.size());

		for (int parent = 0; parent < graph.size(); parent++) {
			graph.forEachChild(parent, [&result](int child, auto&& vertex) {
					result[child] ++;
					});
		}

		return result;
	}
}

struct None {};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

void loop() {
	int n, m;
	std::cin >> n >> m;

	ListGraph<None> graph(n);

	std::vector<int> lookup = Input::inlineToVec<int>(n);

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;

		graph.connect(a - 1, b - 1);
	}

	std::vector<int> dependencies = GraphAlgs::dependencies(graph);
	std::queue<int> queue;
	std::vector<int> depth(n);

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.push(i);
		}
	}

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		graph.forEachChild(parent, [&depth, &dependencies, &queue, &lookup, parent](int child, None& value){
				if (lookup[parent] == lookup[child])
					depth[child] = std::max(depth[child], depth[parent]);
				else
					depth[child] = std::max(depth[child], depth[parent] + 1);
				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(child);
				});
	}

	int endsWith1 = -1;
	int endsWith2 = -1;

	for (int i = 0; i < n; i++) {
		if (lookup[i] == 1)
			endsWith1 = std::max(depth[i], endsWith1);
		else
		 	endsWith2 = std::max(depth[i], endsWith2);
	}

	std::cout << std::min(std::max(endsWith1, endsWith2 + 1), std::max(endsWith2, endsWith1 + 1)) << '\n';
}

int main() {
	FastIO();
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++)
		loop();
}

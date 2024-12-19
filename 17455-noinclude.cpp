#include <algorithm>
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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

enum kdh {
	K,
	D,
	H
};

struct KDH {
	
	kdh ch;

	bool isParent(const KDH other) {
		switch(ch) {
			case K: return other.ch == D;
			case D: return other.ch == H;
			case H: return other.ch == K;
			default: return false;
		};
	}
};

std::istream& operator>>(std::istream& is, KDH& obj) {
	char temp;
	std::cin >> temp;
	switch(temp) {
		case 'K': obj.ch = K; break;
		case 'D': obj.ch = D; break;
		case 'H': obj.ch = H; break;
	}
	return is;
}

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<KDH> graph(Input::inlineToVec<KDH>(n));

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		a --;
		b --;

		if (graph[a].isParent(graph[b]))
			graph.connect(a, b);
		if (graph[b].isParent(graph[a]))
			graph.connect(b, a);
	}

	std::vector<int> dependencies = GraphAlgs::dependencies(graph);

	std::queue<int> queue;

	std::vector<int> depth(n, 0);
	std::vector<bool> ignore(n);

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.push(i);
			switch(graph[i].ch) {
				case K: depth[i] = 1; break;
				case D: depth[i] = 0; break;
				case H: depth[i] = -1; break;
			}
		}
	}

	if (queue.empty()) {
		std::cout << -1;
		return 0;
	}

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		graph.forEachChild(parent, [&queue, &dependencies, &depth, parent](int child, KDH& value) {
				depth[child] = std::max(depth[child], depth[parent] + 1);
				dependencies[child] --;
				if (dependencies[child] == 0) {
					queue.push(child);
				}
				});
	}

	for (int i = 0; i < n; i++) {
		if (dependencies[i] > 0) {
			std::cout << -1;
			return 0;
		}
	}

	int max = *std::max_element(depth.begin(), depth.end());

	if (max < 3)
		std::cout << -1;
	else
		std::cout << (max / 3) * 3;
}

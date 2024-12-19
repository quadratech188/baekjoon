#include <cstdio>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

struct Vec2 {
	int x, y;

	Vec2(int x, int y): x(x), y(y) {}
	Vec2(): x(0), y(0) {}

	Vec2 operator+(const Vec2& other) {
		return Vec2(this->x + other.x, this->y + other.y);
	}

	bool operator==(const Vec2& other) {
		return this->x == other.x && this->y == other.y;
	}
};

template <typename T>
struct Matrix {
	std::vector<T> values;

	int sizeX, sizeY;

	Matrix(int x, int y, int defaultValue = T()) {
		values = std::vector<T>(x * y);
		sizeX = x;
		sizeY = y;
	}

	T& at(int x, int y) {
		return this->values[x * sizeY + y];
	}
	T& operator[](Vec2 index) {
		return this->values[index.x * sizeY + index.y];
	}

	Matrix(): sizeX(0), sizeY(0), values(std::vector<T>()) {}
};

template<typename T>
std::istream& operator>>(std::istream& input, Matrix<T>& matrix) {
	for (int j = 0; j < matrix.sizeY; j++) {
		for (int i = 0; i < matrix.sizeX; i++) {
			std::cin >> matrix.at(i, j);
		}
	}

	return input;
}

template <typename Vertex, typename Edge>
class MatrixGraph {
	std::vector<Vertex> data;
	Matrix<Edge> connections;
	int _size;

public:
	MatrixGraph(int size): data(size), connections(size, size), _size(size) {}
	MatrixGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(values.size(), values.size()),
		_size(values.size()) {}

	Edge& edge(int parent, int child) {
		return connections.at(parent, child);
	}

	void forEachChild(int parent, std::function<void(int, Vertex&)> func) {
		for (int child = 0; child < _size; child++) {
			if (connections.at(parent, child)) {
				func(child, data[child]);
			}
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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	MatrixGraph<int, int> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.edge(a - 1, b - 1) = 1;
	}

	std::vector<int> dependencies = GraphAlgs::dependencies(graph);
	std::queue<int> queue;

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.push(i);
			graph[i] = 1;
		}
	}

	if (queue.empty()) {
		std::cout << 1;
		return 0;
	}

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		graph.forEachChild(parent, [&queue, &dependencies, &graph, parent](int child, int& value) {
				value = std::max(value, graph[parent] + 1);
				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(child);
				});
	}

	int max = 0;
	for (int i = 0; i < n; i++) {
		max = std::max(max, graph[i]);
	}

	std::cout << max;
}

#include <algorithm>
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

int main() {
	int n, k;
	std::cin >> n >> k;

	std::vector<int> times(k, 0);

	MatrixGraph<None, int> graph(k);

	for (int i = 0; i < n; i++) {
		std::string str;
		std::cin >> str;

		for (int a = 0; a < str.size(); a++) {
			for (int b = a + 1; b < str.size(); b++) {
				graph.edge(str[a] - 'A', str[b] - 'A') ++;
			}
		}
	}

	for (int parent = 0; parent < k; parent++) {
		for (int child = 0; child < k; child++) {
			graph.edge(parent, child) = graph.edge(parent, child) == n? 1: 0;
		}
	}

	std::vector<int> dependencies = GraphAlgs::dependencies(graph);

	std::queue<int> queue;
	std::vector<int> depth(k, 0);

	for (int i = 0; i < k; i++) {
		if (dependencies[i] == 0) {
			queue.push(i);
			depth[i] = 1;
		}
	}

	if (queue.empty()) {
		std::cout << 1;
		return 0;
	}

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		graph.forEachChild(parent, [&queue, &depth, &dependencies, &parent](int child, None& value) {
				depth[child] = std::max(depth[child], depth[parent] + 1);

				dependencies[child] --;
				if (dependencies[child] == 0)
					queue.push(child);
				});
	}

	std::cout << *std::max_element(depth.begin(), depth.end()) << '\n';
}

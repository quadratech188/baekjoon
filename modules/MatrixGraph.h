#include "Matrix.h"
#include <functional>
#include <vector>

template <typename Vertex, typename Edge>
class MatrixGraph {
	std::vector<Vertex> data;
	Matrix<Edge> connections;
	int _size;

public:
	MatrixGraph(int size, Vertex defaultV = Vertex()): data(size, defaultV), connections(size, size), _size(size) {}
	MatrixGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(values.size(), values.size()),
		_size(values.size()) {}

	Edge& edge(int parent, int child) {
		return connections.at(parent, child);
	}

	template <typename Callable>
	void forEachChild(int parent, Callable func) {
		for (int child = 0; child < _size; child++) {
			if (connections.at(parent, child)) {
				func(child, data[child], connections.at(parent, child));
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

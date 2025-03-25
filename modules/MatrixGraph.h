#include "Matrix.h"
#include <functional>
#include <ranges>
#include <vector>

template <typename Vertex, typename Edge>
class MatrixGraph {
public:
	using index_t = int;
	using vertex_t = Vertex;
	using edge_t = Edge;
	template <typename T>
	using storage_t = std::vector<T>;
	using size_t = int;

private:
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
		return connections(parent, child);
	}

	size_t size() {
		return _size;
	}

	Vertex& operator[](size_t index) {
		return data[index];
	}

	class child {
	public:
		child(MatrixGraph* graph, index_t parent, index_t self):
			graph(graph), parent(parent), self(self) {}

		child(): graph(nullptr), parent(), self() {}

		index_t index() {
			return self;
		}
		edge_t& edge() {
			return graph->edge(parent, self);
		}
		vertex_t& value() {
			return graph->data[self];
		}

		operator index_t() {
			return self;
		}

	private:
		MatrixGraph* graph;
		index_t parent;
		index_t self;
	};

	auto children(index_t parent) {
		return std::ranges::iota_view(0, _size)
			| std::views::filter([this, parent](index_t index) {
					return this->edge(parent, index);
					})
			| std::views::transform([this, parent](index_t index) {
					return child(this, parent, index);
					});
	}
};

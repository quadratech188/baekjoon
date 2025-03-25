#include <ranges>
#include <variant>

#include "Matrix.h"

template <typename M, typename E = std::monostate>
class GridGraph {
public:
	using index_t = Int2;
	using vertex_t = M::value_type;
	using edge_t = E;
	template <typename T>
	using storage_t = Matrix<T>;
	using size_t = Int2;

	GridGraph(M& data, edge_t const& edge = edge_t()):
		data(data), edge(edge) {}
private:
	M& data;
	edge_t edge;
public:
	vertex_t& operator[](index_t index) {
		return data[index];
	}

	class child {
	public:
		child(GridGraph& graph, index_t index):
			graph(graph), _index(index) {}
	private:
		GridGraph& graph;
		index_t _index;
	public:
		index_t index() {
			return _index;
		}
		edge_t& edge() {
			return graph.edge;
		}
		vertex_t& value() {
			return graph[_index];
		}
		operator index_t() {
			return index();
		}
	};

	auto children(index_t parent) {
		static std::array<index_t, 4> deltas {
			index_t(1, 0),
			index_t(0, 1),
			index_t(-1, 0),
			index_t(0, -1)
		};

		return deltas | std::views::filter([this, parent](index_t delta) {
				return this->data.bounds().contains(parent + delta);
				}) | std::views::transform([this, parent](index_t delta) {
				return child(*this, parent + delta);
				});
	}

	size_t size() const {
		return data.size();
	}
};

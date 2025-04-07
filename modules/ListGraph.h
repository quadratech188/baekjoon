#include <functional>
#include <vector>

template <typename Vertex, typename Edge>
class ListGraph {
public:
	using index_t = int;
	using vertex_t = Vertex;
	using edge_t = Edge;
	template <typename T>
	using storage_t = std::vector<T>;
	using size_t = int;

private:
	std::vector<Vertex> data;
	std::vector<std::vector<std::pair<index_t, edge_t>>> connections;
	size_t _size;

public:
	ListGraph(size_t size, const Vertex& defaultV = Vertex()): data(size, defaultV), connections(size), _size(size) {}
	ListGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(data.size()),
		_size(data.size()) {}

	index_t add(Vertex data) {
		data.push_back(data);
		connections.emplace_back();
		_size += 1;
		return _size - 1;
	}

	void reserve(int size) {
		data.reserve(size);
		connections.reserve(size);
	}

	void connect(index_t parent, index_t child, edge_t edge = edge_t()) {
		connections[parent].emplace_back(child, edge);
	}

	size_t size() const {
		return _size;
	}

	vertex_t& operator[](index_t index) {
		return data[index];
	}

	class child {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = child;

		child(ListGraph* graph, index_t parent, int list_index):
			graph(graph), parent(parent), list_index(list_index) {}

		child():
			graph(nullptr), list_index(0) {}

		index_t index() const {
			return graph->connections[parent][list_index].first;
		}
		edge_t& edge() {
			return graph->connections[parent][list_index].second;
		}
		edge_t const& edge() const {
			return graph->connections[parent][list_index].second;
		}

		vertex_t& value() {
			return graph->data[index()];
		}
		operator index_t() const {
			return index();
		}

		child operator*() const {
			return *this;
		}

		child& operator++() {
			++list_index;
			return *this;
		}
		
		void operator++(int) {
			++list_index;
		}

		bool operator!=(child const& other) const {
			return list_index != other.list_index;
		}

		bool operator==(child const& other) const {
			return list_index == other.list_index;
		}

	private:
		ListGraph* graph;
		index_t parent;
		int list_index;
	};

	class child_range {
	public:
		child_range(ListGraph* graph, index_t parent):
			graph(graph), parent(parent) {}
		child begin() const {
			return child(graph, parent, 0);
		}
		child end() const {
			return child(graph, parent, graph->connections[parent].size());
		}
	private:
		ListGraph* graph;
		index_t parent;
	};

	child_range children(index_t parent) {
		return child_range(this, parent);
	}

	void connect_both(index_t parent, index_t child, edge_t edge1 = edge_t(), edge_t edge2 = edge_t())
	requires std::same_as<int, decltype(edge_t().rev)> {
		edge1.rev = connections[child].size();
		edge2.rev = connections[parent].size();

		connect(parent, child, edge1);
		connect(child, parent, edge2);
	}

	child reverse(child original)
	requires std::same_as<int, decltype(edge_t().rev)> {
		return child(this, original.index(), original.edge().rev);
	}
};

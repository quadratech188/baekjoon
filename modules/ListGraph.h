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

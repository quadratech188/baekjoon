#include <cstddef>
#include <cstdio>
#include <iostream>
#include <queue>
#include <variant>
#include <vector>

template <typename V, typename E, template <typename...> class Container = std::vector>
class ListGraph {
public:
	using index_t = std::size_t;
	using vertex_t = V;
	using edge_t = E;
	template <typename T>
	using storage_t = std::vector<T>;
	using size_t = std::size_t;

	class child {
	public:

		child (index_t index, edge_t edge):
			_index(index), _edge(edge) {}

		child() = default;

		index_t index() {
			return _index;
		}
		edge_t& edge() {
			return _edge;
		}
		edge_t const& edge() const {
			return _edge;
		}

		operator index_t() const {
			return _index;
		}

		bool operator<(child const& other) const {
			return _index < other._index;
		}
	private:
		index_t _index;
		edge_t _edge;
	};

private:
	using connection_list_t = Container<child>;
	std::vector<vertex_t> _data;
	std::vector<connection_list_t> _connections;
	size_t _size;

public:
	ListGraph(size_t size = 0, vertex_t const& default_v = vertex_t()):
		_data(size, default_v), _connections(size), _size(size) {}

	size_t size() const {
		return _size;
	}

	index_t add(vertex_t data) {
		_data.push_back(data);
		_connections.emplace_back();
		_size ++;
		return _size - 1;
	}

	void reserve(size_t size) {
		_data.reserve(size);
		_connections.reserve(size);
	}

	void resize(size_t size) {
		_data.resize(size);
		_connections.resize(size);
		_size = size;
	}

	void connect(index_t parent, index_t child, edge_t edge = edge_t()) {
		if constexpr(requires {_connections[parent].emplace_back(child, edge);})
			_connections[parent].emplace_back(child, edge);
		else
		 	_connections[parent].emplace(child, edge);
	}

	vertex_t& operator[](index_t index) {
		return _data[index];
	}

	auto& children(index_t parent) {
		return _connections[parent];
	}

	size_t degree(index_t parent) const {
		return _connections[parent].size();
	}

	void connect_both(index_t parent, index_t child, edge_t edge1 = edge_t(), edge_t edge2 = edge_t())
	requires std::same_as<int, decltype(edge_t().rev)> {
		edge1.rev = _connections[child].size();
		edge2.rev = _connections[parent].size();

		connect(parent, child, edge1);
		connect(child, parent, edge2);
	}

	child reverse(child original)
	requires std::same_as<int, decltype(edge_t().rev)> {
		return _connections[original.index()][original.edge().rev];
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<std::monostate, std::monostate> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
	}

	std::vector<int> dependencies(n);

	for (int parent = 0; parent < n; parent++) {
		for (auto const& child: graph.children(parent))
			dependencies[child]++;
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

		for (auto const& child: graph.children(parent)) {
			dependencies[child]--;
			if (dependencies[child] == 0)
				queue.push(-child);
		}
	}
}

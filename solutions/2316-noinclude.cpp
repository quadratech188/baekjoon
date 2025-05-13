#include <cstddef>
#include <cstdio>
#include <iostream>
#include <istream>
#include <limits>
#include <queue>
#include <type_traits>
#include <variant>
#include <vector>

namespace Fast {
	class istream: public std::istream {
	public:
		template <typename T>
		inline istream& operator>>(T& val)
		requires std::is_integral_v<T> {
			char ch;
			val = 0;

			do {
				ch = getchar_unlocked();
			} while (ch == ' ' || ch == '\n');

			do {
				val = 10 * val + ch - '0';
				ch = getchar_unlocked();
			} while ('0' <= ch && ch <= '9');

			return *this;
		}
	};

	istream cin;
}

template <typename V, typename E, bool Reversible = false, template <typename...> class Container = std::vector>
class ListGraph {
public:
	// Builder
	template <bool value>
	using reversible = ListGraph<V, E, value, Container>;
	template <template <typename...> class value>
	using container = ListGraph<V, E, Reversible, value>;

	static constexpr bool reversible_v = Reversible;
	template <typename T>
	using container_t = Container<T>;

	using index_t = std::size_t;
	using vertex_t = V;
	using edge_t = E;
	template <typename T>
	using storage_t = std::vector<T>;
	using size_t = std::size_t;

	class child {
		friend class ListGraph;

	public:
		child(index_t index, edge_t edge, index_t rev) noexcept:
			_index(index), _edge(edge), _rev(rev) {}

		child (index_t index, edge_t edge) noexcept:
			_index(index), _edge(edge) {}

		child() = default;

		inline index_t index() const noexcept {
			return _index;
		}
		inline edge_t& edge() noexcept {
			return _edge;
		}
		inline edge_t const& edge() const noexcept {
			return _edge;
		}

		inline operator index_t() const noexcept {
			return _index;
		}

		inline bool operator<(child const& other) const noexcept {
			return _index < other._index;
		}
	private:
		index_t _index;
		edge_t _edge;
		std::conditional_t<reversible_v, index_t, std::monostate> _rev;
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

	void reserve_children(size_t size) {
		for (auto& connection: _connections)
			connection.reserve(size);
	}

	void resize(size_t size) {
		_data.resize(size);
		_connections.resize(size);
		_size = size;
	}

	void connect(index_t parent, index_t child, edge_t edge = edge_t()) {
		if constexpr (requires {_connections[parent].emplace_back(child, edge);})
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
	requires reversible_v {
		_connections[parent].emplace_back(child, edge1, _connections[child].size());
		_connections[child].emplace_back(parent, edge2, _connections[parent].size() - 1);
	}

	child& reverse(child const& original)
	requires reversible_v {
		return _connections[original._index][original._rev];
	}
};

int main() {
	size_t n, p;
	Fast::cin >> n >> p;

	ListGraph<std::monostate, int>
		::reversible<true> graph(2 * n);

	for (size_t i = 0; i < n; i++) {
		graph.connect_both(2 * i, 2 * i + 1, 1, 0);
	}

	for (size_t i = 0; i < p; i++) {
		size_t a, b;
		Fast::cin >> a >> b;
		graph.connect_both(2 * a - 1, 2 * b - 2, std::numeric_limits<int>::max(), 0);
		graph.connect_both(2 * b - 1, 2 * a - 2, std::numeric_limits<int>::max(), 0);
	}

	size_t const source = 2 * 0 + 1;
	size_t const sink = 2 * 1;
	size_t const size = 2 * n;

	std::vector<size_t> prev(size);
	std::vector<decltype(graph)::child*> edges(size);
	std::queue<size_t> queue;

	int count = 0;

	while (true) {
		std::fill(prev.begin(), prev.end(), std::numeric_limits<size_t>::max());

		queue.push(source);

		while (!queue.empty()) {
			size_t parent = queue.front();
			queue.pop();

			for (auto& child: graph.children(parent)) {
				if (child.edge() > 0 && prev[child] == std::numeric_limits<size_t>::max()) {
					prev[child] = parent;
					edges[child] = &child;
					queue.push(child);
				}
			}
		}
		if (prev[sink] == std::numeric_limits<size_t>::max()) break;

		for (size_t i = sink; i != source; i = prev[i]) {
			edges[i]->edge() --;
			graph.reverse(*edges[i]).edge() ++;
		}
		count++;
	}
	std::cout << count;
}

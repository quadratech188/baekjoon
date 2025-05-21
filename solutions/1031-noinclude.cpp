#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <istream>
#include <limits>
#include <queue>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>
#include <variant>
#include <vector>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

#ifndef FASTIO_BUFFER_SIZE
#define FASTIO_BUFFER_SIZE 1 << 20
#endif

namespace Fast {
	class istream {
	private:
		char getchar() {
			static char buffer[FASTIO_BUFFER_SIZE];
			static char* ptr = buffer;
			static char* end = buffer;

			if (ptr == end) {
				ssize_t size = read(STDIN_FILENO, buffer, sizeof(buffer));
				ptr = buffer;
				end = buffer + size;
			}
			return *(ptr++);
		}
	public:
		template <typename T>
		inline istream& operator>>(T& val)
		requires std::is_integral_v<T> {
			char ch;
			val = 0;

			do {
				ch = getchar();
			} while (ch == ' ' || ch == '\n');

			do {
				val = 10 * val + ch - '0';
				ch = getchar();
			} while ('0' <= ch && ch <= '9');

			return *this;
		}
	};

	istream cin;
}

template <typename V, typename E, bool Reversible = false, template <typename...> class Container = std::vector, typename Index = std::size_t>
class ListGraph {
public:
	// Builder
	template <bool value>
	using with_reversible = ListGraph<V, E, value, Container, Index>;
	template <template <typename...> class value>
	using with_container = ListGraph<V, E, Reversible, value, Index>;
	template <typename value>
	using with_index = ListGraph<V, E, Reversible, Container, value>;

	static constexpr bool reversible_v = Reversible;
	template <typename T>
	using container_t = Container<T>;

	using index_t = Index;
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
		[[no_unique_address]]
		edge_t _edge;
		[[no_unique_address]]
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
	uint n, m;
	Fast::cin >> n >> m;

	size_t const size = n + m + 2;
	uint const source = n + m;
	uint const sink = n + m + 1;

	ListGraph<std::monostate, int>
		::with_reversible<true>
		::with_index<uint> graph(size);

	int n_sum = 0;
	for (uint i = 0; i < n; i++) {
		int k;
		Fast::cin >> k;
		graph.connect_both(source, i, k, 0);
		n_sum += k;
	}

	int m_sum = 0;
	for (uint j = n; j < n + m; j++) {
		int k;
		Fast::cin >> k;
		graph.connect_both(j, sink, k, 0);
		m_sum += k;
	}

	if (n_sum != m_sum) {
		std::cout << "-1";
		return 0;
	}

	for (uint i = 0; i < n; i++) {
		for (uint j = n; j < n + m; j++)
			graph.connect_both(i, j, 1, 0);
	}

	std::vector<uint> prev(size);
	std::vector<decltype(graph)::child*> edges(size);
	std::queue<uint> queue;

	int flow = 0;
	while (true) {
		std::fill(prev.begin(), prev.end(), std::numeric_limits<uint>::max());

		queue.push(source);

		while (!queue.empty()) {
			uint parent = queue.front();
			queue.pop();

			for (auto& child: graph.children(parent)) {
				if (child.edge() == 0 || prev[child] != std::numeric_limits<uint>::max()) continue;

				prev[child] = parent;
				edges[child] = &child;
				queue.push(child);
			}
		}

		if (prev[sink] == std::numeric_limits<uint>::max()) break;

		for (uint i = sink; i != source; i = prev[i]) {
			edges[i]->edge() --;
			graph.reverse(*edges[i]).edge() ++;
		}
		flow++;
	}

	if (n_sum != flow) {
		std::cout << "-1";
		return 0;
	}

	for (uint a = 0; a < n; a++) {
		for (auto& b: graph.children(a)) {
			if (b == source) continue;

			// unused
			if (b.edge() == 1) continue;

			std::fill(prev.begin(), prev.end(), std::numeric_limits<uint>::max());

			queue.push(a);
			
			while (!queue.empty()) {
				uint parent = queue.front();
				queue.pop();

				if (parent < a) continue;

				for (auto& child: graph.children(parent)) {
					if (child.edge() == 0 || prev[child] != std::numeric_limits<uint>::max()) continue;
					if (parent == a && child < b) continue;

					prev[child] = parent;
					edges[child] = &child;
					queue.push(child);
				}
			}

			if (prev[b] == std::numeric_limits<uint>::max()) continue;

			for (uint i = b; i != a; i = prev[i]) {
				edges[i]->edge() --;
				graph.reverse(*edges[i]).edge() ++;
			}

			b.edge() ++;
			graph.reverse(b).edge() --;
		}
	}

	for (uint i = 0; i < n; i++) {
		for (auto& child: graph.children(i)) {
			if (child == source) continue;
			std::cout << 1 - child.edge();
		}
		std::cout << '\n';
	}
}

#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <istream>
#include <type_traits>
#include <unistd.h>
#include <utility>
#include <vector>

#ifndef FASTISTREAM_BUFFER_SIZE
#define FASTISTREAM_BUFFER_SIZE 1 << 20
#endif

#ifndef FASTOSTREAM_BUFFER_SIZE
#define FASTOSTREAM_BUFFER_SIZE 1 << 20
#endif

namespace Fast {
	class istream {
	private:
		inline char getchar() {
			static char buffer[FASTISTREAM_BUFFER_SIZE];
			static char* ptr = buffer;
			static char* end = buffer;

			if (ptr == end) {
				ssize_t size = read(STDIN_FILENO, buffer, sizeof(buffer));
				if (size <= 0) return EOF;
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
			} while (std::isspace(ch));

			// Optimized away for non-signed types
			bool negative = false;
			if constexpr (std::is_signed_v<T>) {
				if (ch == '-') {
					negative = true;
					ch = getchar();
				}
			}

			do {
				val = 10 * val + ch - '0';
				ch = getchar();
			} while ('0' <= ch && ch <= '9');

			if constexpr (std::is_signed_v<T>)
				if (negative) val = -val;

			return *this;
		}

		inline istream& operator>>(char& val) {
			do {
				val = getchar();
			} while (std::isspace(val));
			return *this;
		}

		template <typename T1, typename T2>
		inline istream& operator>>(std::pair<T1, T2>& pair) {
			(*this) >> pair.first >> pair.second;
			return *this;
		}

		template <typename T>
		std::vector<T> to_vec(uint size) {
			std::vector<T> result(size);

			for (auto& val: result)
				(*this) >> val;

			return result;
		}
	};

	istream cin;
}
#ifdef HACKABLE
#define hackable_private public
#else
#define hackable_private private
#endif

#include <cstddef>
#include <type_traits>
#include <variant>
#include <vector>

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
	hackable_private:
		index_t _index;
		[[no_unique_address]]
		edge_t _edge;
		[[no_unique_address]]
		std::conditional_t<reversible_v, index_t, std::monostate> _rev;
	};

hackable_private:
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

	index_t add(vertex_t data = {}) {
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
#include <limits>
#include <queue>
#include <variant>

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	ListGraph<std::monostate, int> graph(n);

	std::vector<int> dependencies(n);

	for (uint i = 0; i < m; i++) {
		uint a, b;
		int t;
		Fast::cin >> a >> b >> t;

		graph.connect(a - 1, b - 1, t);
		dependencies[b - 1] ++;
	}

	uint start, end;
	Fast::cin >> start >> end;
	start --;
	end --;

	std::queue<uint> queue;

	std::vector<int> distances(n);

	std::vector<std::vector<uint>> parents(n);

	queue.push(start);
	while (!queue.empty()) {
		uint parent = queue.front();
		queue.pop();

		for (auto& child: graph.children(parent)) {
			if (distances[parent] + child.edge() > distances[child]) {
				parents[child].clear();
				parents[child].push_back(parent);

				distances[child] = distances[parent] + child.edge();
			}
			else if (distances[parent] + child.edge() == distances[child])
				parents[child].push_back(parent);

			dependencies[child] --;

			if (dependencies[child] == 0)
				queue.push(child);
		}
	}

	std::vector<bool> visited(n, false);
	int cnt = 0;

	queue.push(end);
	while (!queue.empty()) {
		uint child = queue.front();
		queue.pop();

		for (uint parent: parents[child]) {
			cnt ++;

			if (visited[parent]) continue;
			queue.push(parent);
			visited[parent] = true;
		}
	}

	std::cout << distances[end] << '\n' << cnt;
}

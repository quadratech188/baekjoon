#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <istream>
#include <ranges>
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

		template <typename... Ts>
		std::tuple<Ts...> get() {
			std::tuple<Ts...> result;

			std::apply([this](auto&... x) {(((*this) >> x), ...);}, result);

			return result;
		}

		template <typename T>
		std::vector<T> to_vec(uint size) {
			std::vector<T> result(size);

			for (auto& val: result)
				(*this) >> val;

			return result;
		}

		template <typename T>
		auto to_range(uint size) {
			return std::views::iota(0u, size)
				| std::views::transform([this](uint) {
						T temp;
						(*this) >> temp;
						return temp;
						});
		}
	};

	istream cin;
}
#ifdef HACKABLE
#define hackable_private public
#else
#define hackable_private private
#endif

#include <algorithm>
#include <cstddef>
#include <ranges>
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

hackable_private:
	struct child {
		index_t _index;
		[[no_unique_address]]
		edge_t _edge;
		[[no_unique_address]]
		std::conditional_t<reversible_v, index_t, std::monostate> _rev;
	};

	using connection_list_t = Container<child>;
	std::vector<vertex_t> _data;
	std::vector<connection_list_t> _connections;
	size_t _size;

public:
	struct child_ref {
		friend ListGraph;

		index_t index() const {
			return _ref._index;
		}
		edge_t& edge() {
			return _ref._edge;
		}
		operator index_t() const {
			return _ref._index;
		}

	hackable_private:
		child_ref(child& ref):
			_ref(ref) {}

		child& _ref;
	};

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

	auto children(index_t parent) {
		return _connections[parent]
			| std::views::transform([](child& ref) -> child_ref {return {ref};});
	}

	size_t degree(index_t parent) const {
		return _connections[parent].size();
	}

	void connect_both(index_t parent, index_t child, edge_t edge1 = edge_t(), edge_t edge2 = edge_t())
	requires reversible_v {
		_connections[parent].emplace_back(child, edge1, _connections[child].size());
		_connections[child].emplace_back(parent, edge2, _connections[parent].size() - 1);
	}

	child_ref reverse(child_ref original)
	requires reversible_v {
		return {_connections[original._ref._index][original._ref._rev]};
	}
};
#include <variant>

// https://en.wikipedia.org/wiki/Biconnected_component#Linear_time_depth-first_search

ListGraph<std::monostate, std::monostate> graph;

std::vector<bool> visited;
std::vector<uint> depth, low, parent;

std::vector<uint> articulations;

void GetArticulationPoints(uint i, uint d) {
	visited[i] = true;
	depth[i] = d;
	low[i] = d;
	uint childCount = 0;
	bool isArticulation = false;

	for (auto ni: graph.children(i)) {
		if (!visited[ni]) {
			parent[ni] = i;
			GetArticulationPoints(ni, d + 1);
			childCount = childCount + 1;
			if (low[ni] >= depth[i])
				isArticulation = true;
			low[i] = std::min(low[i], low[ni]);
		}
		else if (ni != parent[i])
			low[i] = std::min(low[i], depth[ni]);
	}
	if ((parent[i] != i && isArticulation) || (parent[i] == i && childCount > 1))
		articulations.push_back(i);
}

int main() {
	uint v, e;
	Fast::cin >> v >> e;

	graph.resize(v);
	visited.resize(v);
	depth.resize(v);
	low.resize(v);
	parent.resize(v);

	for (uint i = 0; i < e; i++) {
		uint a, b;
		Fast::cin >> a >> b;
		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
	}

	for (uint i = 0; i < v; i++) {
		if (!visited[i]) {
			parent[i] = i;
			GetArticulationPoints(i, 0);
		}
	}

	std::ranges::sort(articulations);
	std::cout << articulations.size() << '\n';
	for (auto val: articulations)
		std::cout << val + 1 << ' ';
}

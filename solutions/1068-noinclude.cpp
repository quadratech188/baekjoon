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
		child_ref() {}

		friend ListGraph;

		index_t index() const {
			return _ref->_index;
		}
		edge_t& edge() {
			return _ref->_edge;
		}
		operator index_t() const {
			return _ref->_index;
		}

	hackable_private:
		child_ref(child* ref):
			_ref(ref) {}

		child* _ref;
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

	void resize(size_t size, vertex_t const& new_vertex) {
		_data.resize(size, new_vertex);
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

	vertex_t& safe_at(index_t index, vertex_t const& new_vertex = {}) {
		if (index >= size())
			resize(index + 1, new_vertex);
		return _data[index];
	}

	auto children(index_t parent) {
		return _connections[parent]
			| std::views::transform([](child& ref) -> child_ref {return {&ref};});
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
		return {&_connections[original._ref->_index][original._ref->_rev]};
	}
};
#include <ranges>
#include <unistd.h>
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
		char buffer[FASTISTREAM_BUFFER_SIZE];
		char* ptr = buffer;
		char* end = buffer;

		inline char getchar() {
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
		inline istream& operator>>(std::tuple<Ts...>& tuple){
			std::apply([this](auto&... x) {(((*this) >> x), ...);}, tuple);
			return *this;
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

#include <concepts>
#include <ranges>

template <typename G>
concept Graph = requires(G graph,
		typename G::child_ref child,
		typename G::index_t u,
		typename G::vertex_t v,
		typename G::edge_t e) {

	typename G::index_t;
	typename G::vertex_t;
	typename G::edge_t;
	typename G::template storage_t<int>;
	typename G::size_t;
	typename G::child_ref;

	{child.index()} -> std::same_as<typename G::index_t>;
	{child.edge()} -> std::same_as<typename G::edge_t&>;
	requires std::convertible_to<typename G::child_ref, typename G::index_t>;

	{graph.size()} -> std::same_as<typename G::size_t>;

	{graph.children(u)} -> std::ranges::range;
	requires std::same_as<std::ranges::range_value_t<decltype(graph.children(u))>, typename G::child_ref>;
};
#include <variant>
#include <iostream>

template <Graph G>
uint dfs(G& graph, uint parent, uint ignore) {
	if (parent == ignore) return 0;
	uint result = 0;

	for (auto child: graph.children(parent)) {
		result += dfs(graph, child, ignore);
	}
	if (result == 0) result = 1;
	return result;
}

int main() {
	uint n;
	Fast::cin >> n;
	uint root;
	ListGraph<std::monostate, std::monostate> graph(n);

	for (uint i = 0; i < n; i++) {
		int x;
		Fast::cin >> x;
		if (x == -1) {
			root = i;
		}
		else {
			graph.connect(x, i);
		}
	}
	uint del;
	Fast::cin >> del;

	std::cout << dfs(graph, root, del);
}

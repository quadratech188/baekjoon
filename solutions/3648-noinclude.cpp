#include <iostream>
#include <cstdio>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
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
#include <limits>
#include <stack>
#include <vector>

template <Graph G, typename B>
class SSC {
public:
	SSC(G& graph, B& bins):
		graph(graph), bins(bins),
		indices(graph.size(), std::numeric_limits<uint>::max()),
		lowlinks(graph.size(), std::numeric_limits<uint>::max()),
		on_stack(graph.size(), false),
		index(0),
		S() {}

	void operator()() {
		for (typename G::index_t i = 0; i < graph.size(); i++) {
			if (indices[i] == std::numeric_limits<uint>::max())
				strongconnect(i);
		}
	}

hackable_private:
	// https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
	G& graph;
	B& bins;

	G::template storage_t<uint> indices, lowlinks;
	G::template storage_t<bool> on_stack;

	uint index;
	std::stack<typename G::index_t> S;

	void strongconnect(G::index_t v) {
		indices[v] = index;
		lowlinks[v] = index;
		index ++;
		S.push(v);
		on_stack[v] = true;

		for (auto w: graph.children(v)) {
			if (indices[w] ==std::numeric_limits<uint>::max()) {
				strongconnect(w);
				lowlinks[v] = std::min(lowlinks[v], lowlinks[w]);
			}
			else if (on_stack[w]) {
				lowlinks[v] = std::min(lowlinks[v], indices[w]);
			}
		}

		if (indices[v] == lowlinks[v]) {
			bins.new_bin();

			typename G::index_t w;
			do {
				w = S.top();
				S.pop();
				on_stack[w] = false;
				bins.insert(w);
			} while (w != v);
		}
	}
};
#include <set>
#include <sys/types.h>
#include <vector>

template <typename T>
struct SetBins {
public:
	void new_bin() {
		bins.emplace_back();
	}
	void insert(T val) {
		bins.back().insert(val);
	}
	std::vector<std::set<T>> bins;
};

struct IdBins {
public:
	IdBins(uint size):
		group(size, -1), bin_counter(-1) {}

	void new_bin() {
		bin_counter ++;
	}
	void insert(uint val) {
		group[val] = bin_counter;
	}

	int operator[](uint i) {
		return group[i];
	}

	uint size() {
		return bin_counter;
	}

	std::vector<int> group;
	int bin_counter;
};
#include <iostream>
#include <variant>

bool loop() {
	FastIO();
	uint n, m;
	std::cin >> n >> m;

	if (std::cin.eof()) return false;

	ListGraph<std::monostate, std::monostate> graph(2 * n);

	auto to_index = [n](int i) -> uint {
		if (i > 0) return i - 1;

		return n - i - 1;
	};

	for (uint _ = 0; _ < m; _++) {
		int i, j;
		std::cin >> i >> j;

		graph.connect(to_index(-i), to_index(j));
		graph.connect(to_index(-j), to_index(i));
	}
	graph.connect(n, 0);

	IdBins ssc_groups(graph.size());

	SSC(graph, ssc_groups)();

	for (uint i = 0; i < n; i++) {
		if (ssc_groups[i] == ssc_groups[n + i]) {
			std::cout << "no\n";
			return true;
		}
	}
	std::cout << "yes\n";
	return true;
}

int main() {
	while (loop()) {}
}

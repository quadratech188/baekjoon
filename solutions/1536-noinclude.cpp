#include <concepts>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <limits>
#include <queue>
#include <ranges>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

template <typename G>
concept Graph = requires(G graph, typename G::index_t u, typename G::vertex_t v, typename G::edge_t e) {
	typename G::index_t;
	typename G::vertex_t;
	typename G::edge_t;
	typename G::template storage_t<int>;
	typename G::size_t;

	{graph[u]} -> std::same_as<typename G::vertex_t&>;

	{graph.children(u)} -> std::ranges::range;

	{graph.size()} -> std::same_as<typename G::size_t>;

	requires std::constructible_from<typename G::template storage_t<int>, typename G::size_t>;

	requires requires (std::ranges::range_value_t<decltype(graph.children(u))> child) {
		{child.index()} -> std::same_as<typename G::index_t>;
		{child.edge()} -> std::same_as<typename G::edge_t&>;
		{child} -> std::convertible_to<typename G::index_t>;
	};
};

template <typename V, typename E, bool Reversible = false, template <typename...> class Container = std::vector>
class ListGraph {
public:
	// Builder
	template <bool value>
	using reversible = ListGraph<V, E, value, Container>;
	template <template <typename...> class value>
	using container = ListGraph<V, E, Reversible, value>;

	static constexpr bool reversible_v = Reversible;
	template <typename... Args>
	using container_t = Container<Args...>;

	using index_t = std::size_t;
	using vertex_t = V;
	using edge_t = E;
	template <typename T>
	using storage_t = std::vector<T>;
	using size_t = std::size_t;

	class child {
		friend class ListGraph;

	public:
		child(index_t index, edge_t edge, index_t rev) noexcept
			requires reversible_v:
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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

struct Edge {
	int cap = 0;
	int flow = 0;

	int residual() const {
		return cap - flow;
	}
};

template <Graph G>
bool check(size_t r, size_t n, G& graph) {
	size_t const size = 6 * n + 2;
	size_t const source = 6 * n;
	size_t const sink = 6 * n + 1;

	// Reset all flows to 0
	for (size_t parent = 0; parent < graph.size(); ++parent) {
		for (auto& child : graph.children(parent)) {
			child.edge().flow = 0;
		}
	}

	// Update only capacities that depend on r
	for (auto& child : graph.children(source)) {
		child.edge().cap = r;
	}
	for (size_t i = 0; i < n; ++i) {
		for (auto& child : graph.children(3 * n + i)) {
			child.edge().cap = r;
		}
	}

	std::vector<size_t> prev(size);
	std::vector<typename G::child*> edges(size);
	std::queue<size_t> queue;

	size_t total_flow = 0;
	while (true) {
		std::fill(prev.begin(), prev.end(), std::numeric_limits<size_t>::max());

		queue.push(source);
		while (!queue.empty()) {
			size_t cur = queue.front(); queue.pop();
			for (auto& child : graph.children(cur)) {
				if (child.edge().residual() > 0 && prev[child] == std::numeric_limits<size_t>::max()) {
					prev[child] = cur;
					edges[child] = &child;
					queue.push(child);
				}
			}
		}

		if (prev[sink] == std::numeric_limits<size_t>::max()) break;

		int flow = std::numeric_limits<int>::max();
		for (size_t i = sink; i != source; i = prev[i]) {
			flow = std::min(flow, edges[i]->edge().residual());
		}
		for (size_t i = sink; i != source; i = prev[i]) {
			edges[i]->edge().flow += flow;
			graph.reverse(*edges[i]).edge().flow -= flow;
		}
		total_flow += flow;
	}

	return total_flow == r * n;
}

int main() {
	FastIO();
	size_t n;
	int k;
	std::cin >> n >> k;

	size_t const size = 6 * n + 2;
	size_t const source = 6 * n;
	size_t const sink = 6 * n + 1;
	int const INF = std::numeric_limits<int>::max();

	ListGraph<std::monostate, Edge>::reversible<true> graph(size);

	for (size_t i = 0; i < n; i++) {
		graph.connect_both(source, i, {0, 0}, {0, 0});
		graph.connect_both(i, n + i, {INF, 0}, {0, 0});
		graph.connect_both(i, 2 * n + i, {k, 0}, {0, 0});
		graph.connect_both(3 * n + i, sink, {0, 0}, {0, 0});
		graph.connect_both(4 * n + i, 3 * n + i, {INF, 0}, {0, 0});
		graph.connect_both(5 * n + i, 3 * n + i, {k, 0}, {0, 0});
	}

	for (size_t boy = 0; boy < n; boy++) {
		for (size_t girl = 0; girl < n; girl++) {
			char type;
			std::cin >> type;
			if (type == '1') {
				graph.connect_both(n + boy, 4 * n + girl, {1, 0}, {0, 0});
			} else {
				graph.connect_both(2 * n + boy, 5 * n + girl, {1, 0}, {0, 0});
			}
		}
	}

	size_t l = 0, r = n, ans = 0;
	while (l <= r) {
		size_t mid = (l + r) / 2;
		if (check(mid, n, graph)) {
			ans = mid;
			l = mid + 1;
		} else {
			r = mid - 1;
		}
	}

	std::cout << ans;
}

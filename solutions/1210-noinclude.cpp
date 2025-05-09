#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <limits>
#include <queue>
#include <type_traits>
#include <variant>
#include <vector>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
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

int main() {
	FastIO();
	size_t n, m;
	std::cin >> n >> m;

	size_t source, sink;
	std::cin >> source >> sink;
	source = 2 * source - 2;
	sink = 2 * sink - 1;

	size_t const size = 2 * n;

	ListGraph<std::monostate, uint64_t>
		::reversible<true> graph(size);

	for (size_t i = 0; i < n; i++) {
		uint64_t cost;
		std::cin >> cost;

		graph.connect_both(2 * i, 2 * i + 1, cost, 0);
	}

	for (size_t i = 0; i < m; i++) {
		size_t a, b;
		std::cin >> a >> b;
		graph.connect_both(2 * a - 1, 2 * b - 2, std::numeric_limits<uint64_t>::max(), 0);
		graph.connect_both(2 * b - 1, 2 * a - 2, std::numeric_limits<uint64_t>::max(), 0);
	}

	std::vector<size_t> prev;
	std::vector<decltype(graph)::child*> edges(size);
	std::queue<size_t> queue;

	while (true) {
		prev.assign(size, std::numeric_limits<size_t>::max());
		queue.push(source);

		while (!queue.empty()) {
			size_t parent = queue.front();
			queue.pop();

			for (auto& child: graph.children(parent)) {
				if (prev[child] == std::numeric_limits<size_t>::max() && child.edge() > 0) {
					prev[child] = parent;
					edges[child] = &child;

					queue.push(child);
				}
			}
		}

		if (prev[sink] == std::numeric_limits<size_t>::max()) break;

		uint64_t max_flow = std::numeric_limits<uint64_t>::max();

		for (size_t ptr = sink; ptr != source; ptr = prev[ptr]) {
			if (edges[ptr]->edge() < max_flow)
				max_flow = edges[ptr]->edge();
		}

		for (size_t ptr = sink; ptr != source; ptr = prev[ptr]) {
			edges[ptr]->edge() -= max_flow;
			graph.reverse(*edges[ptr]).edge() += max_flow;
		}
	}

	std::vector<size_t> answers;
	std::vector<bool> visited(size, false);

	queue.push(source);
	visited[source] = true;

	while (!queue.empty()) {
		size_t parent = queue.front();
		queue.pop();

		for (auto& child: graph.children(parent)) {
			if (!visited[child] && child.edge() > 0) {
				visited[child] = true;
				queue.push(child);
			}
		}
	}
	for (size_t i = 0; i < n; i++) {
		if (visited[2 * i] && !visited[2 * i + 1])
			std::cout << i + 1 << ' ';
	}
}

#include <cstddef>
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

template <bool Reversibility = false, template <typename...> class Container = std::vector>
struct ListGraphConfig {
	static constexpr bool reversible_v = Reversibility;
	template <typename T>
	using container_t = Container<T>;

	template <bool value>
	using reversible = ListGraphConfig<value, Container>;

	template <template <typename...> class value>
	using container = ListGraphConfig<Reversibility, value>;
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
		child (index_t index, edge_t edge):
			_index(index), _edge(edge) {}

		child() = default;

		index_t index() const {
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

		auto& rev() requires reversible_v {
			return _rev;
		}
	private:
		std::conditional_t<reversible_v, index_t, std::monostate> _rev;
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

	child& connect(index_t parent, index_t child, edge_t edge = edge_t()) {
		if constexpr(requires {_connections[parent].emplace_back(child, edge);})
			return _connections[parent].emplace_back(child, edge);
		else
		 	return _connections[parent].emplace(child, edge);
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
		class child& child1 = connect(parent, child, edge1);
		class child& child2 = connect(child, parent, edge2);

		child1._rev = _connections[child].size() - 1;
		child2._rev = _connections[parent].size() - 1;
	}

	child& reverse(child const& original)
	requires reversible_v {
		return _connections[original.index()][original._rev];
	}
};

struct Edge {
	Edge(): capacity(0), cost(0) {}
	Edge(int capacity, int cost):
		capacity(capacity), cost(cost) {}

	int capacity;
	int cost;
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	const int size = 2 * n + 2;
	const int source = 2 * n;
	const int sink = 2 * n + 1;

	ListGraph<std::monostate, Edge>::reversible<true> graph(size);

	for (int i = 0; i < n; i++) {
		graph.connect_both(source, i, Edge(1, 0), Edge(0, 0));
	}

	for (int i = n; i < 2 * n; i++) {
		graph.connect_both(i, sink, Edge(1, 0), Edge(0, 0));
	}

	for (int i = 0; i < n; i++) {
		for (int j = n; j < 2 * n; j++) {
			int cost;
			std::cin >> cost;
			graph.connect_both(i, j, Edge(1, cost), Edge(0, -cost));
		}
	}

	int result = 0;
	
	while (true) {
		std::vector<int> prev(size, -1), dist(size, std::numeric_limits<int>::max());
		std::vector<decltype(graph)::child*> edges(size);
		std::vector<bool> in_queue(size, false);
		std::queue<int> queue;
		dist[source] = 0;
		in_queue[source] = true;
		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			in_queue[parent] = false;
			for (auto& child: graph.children(parent)) {
				if (child.edge().capacity > 0
						&& dist[child] > dist[parent] + child.edge().cost) {
					dist[child] = dist[parent] + child.edge().cost;
					prev[child] = parent;
					edges[child] = &child;

					if (!in_queue[child]) {
						queue.push(child);
						in_queue[child] = true;
					}
				}
			}
		}

		if (prev[sink] == -1) break;

		int max_flow = std::numeric_limits<int>::max();

		for (int i = sink; i != source; i = prev[i])
			max_flow = std::min(max_flow, edges[i]->edge().capacity);

		for (int i = sink; i != source; i = prev[i]) {
			result += max_flow * edges[i]->edge().cost;
			edges[i]->edge().capacity -= max_flow;
			graph.reverse(*edges[i]).edge().capacity += max_flow;
		}
	}

	std::cout << result;
}

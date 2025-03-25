#include <concepts>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <ranges>
#include <utility>
#include <variant>
#include <vector>
template <typename G>
concept Graph = requires(G graph, typename G::index_t u, typename G::vertex_t v, typename G::edge_t e) {
	typename G::index_t;
	typename G::vertex_t;
	typename G::edge_t;

	{graph[u]} -> std::convertible_to<typename G::vertex_t&>;

	{graph.children(u)} -> std::ranges::range;

	requires requires (std::ranges::range_value_t<decltype(graph.children(u))> child) {
		{child.index()} -> std::convertible_to<typename G::index_t>;
		{child.edge()} -> std::convertible_to<typename G::edge_t&>;
		{child.value()} -> std::convertible_to<typename G::vertex_t&>;
		{child} -> std::convertible_to<typename G::index_t>;
	};
};

template <typename Vertex, typename Edge>
class ListGraph {
public:
	using index_t = int;
	using vertex_t = Vertex;
	using edge_t = Edge;

private:
	std::vector<Vertex> data;
	std::vector<std::vector<std::pair<index_t, edge_t>>> connections;
	int _size;

public:

	ListGraph(int size, const Vertex& defaultV = Vertex()): data(size, defaultV), connections(size), _size(size) {}
	ListGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(data.size()),
		_size(data.size()) {}

	index_t add(Vertex data) {
		data.push_back(data);
		connections.emplace_back();
		_size += 1;
		return _size - 1;
	}

	void reserve(int size) {
		data.reserve(size);
		connections.reserve(size);
	}

	void connect(index_t parent, index_t child, edge_t edge = edge_t()) {
		connections[parent].push_back(std::make_pair(child, edge));
	}

	size_t size() {
		return _size;
	}

	vertex_t& operator[](index_t index) {
		return data[index];
	}

	class child {
	public:
		child(ListGraph* graph, index_t parent, int list_index):
			graph(graph), parent(parent), list_index(list_index) {}

		child():
			graph(nullptr), list_index(0) {}

		index_t index() {
			return graph->connections[parent][list_index].first;
		}
		edge_t& edge() {
			return graph->connections[parent][list_index].second;
		}
		vertex_t& value() {
			return graph->data[index()];
		}

		operator index_t() {
			return index();
		}

	private:
		ListGraph* graph;
		index_t parent;
		int list_index;
	};

	auto children(index_t parent) {
		return std::ranges::iota_view(static_cast<size_t>(0), connections[parent].size())
			| std::views::transform([this, parent](index_t index) {
					return child(this, parent, index);
					});
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

	ListGraph<std::monostate, int> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b, d;
		std::cin >> a >> b >> d;
		graph.connect(a - 1, b - 1, d);
		graph.connect(b - 1, a - 1, d);
	}

	std::priority_queue<std::tuple<int, int, int>> queue;
	std::vector<int> fox_times(n, std::numeric_limits<int>::max());
	fox_times[0] = 0;
	queue.emplace(0, 0, 0);

	while (!queue.empty()) {
		auto [time, parent, turn] = queue.top();
		time = - time;
		queue.pop();

		if (fox_times[parent] < time) continue;

		for (auto child: graph.children(parent)) {
			int next_time = time + child.edge() * 2;
			if (fox_times[child] > next_time) {
				fox_times[child] = next_time;
				queue.emplace(- next_time, child, turn + 1);
			}
		}
	}

	std::array<std::vector<int>, 2> wolf_times;
	wolf_times[0].assign(n, std::numeric_limits<int>::max());
	wolf_times[1].assign(n, std::numeric_limits<int>::max());
	wolf_times[0][0] = 0;
	queue.emplace(0, 0, 0);

	while (!queue.empty()) {
		auto [time, parent, turn] = queue.top();
		time = - time;
		queue.pop();

		if (wolf_times[turn % 2][parent] < time) continue;

		if (turn % 2 == 0) {
			for (auto child: graph.children(parent)) {
				int next_time = time + child.edge();
				if (wolf_times[1][child] > next_time) {
					wolf_times[1][child] = next_time;
					queue.emplace(- next_time, child, turn + 1);
				}
			}
		}
		else {
			for (auto child: graph.children(parent)) {
				int next_time = time + 4 * child.edge();
				if (wolf_times[0][child] > next_time) {
					wolf_times[0][child] = next_time;
					queue.emplace(- next_time, child, turn + 1);
				}
			}
		}
	}
	
	int count = 0;
	for (int i = 1; i < n; i++) {
		if (fox_times[i] < std::min(wolf_times[0][i], wolf_times[1][i])) count++;
	}

	std::cout << count;
}

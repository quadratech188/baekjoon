#include <concepts>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <queue>
#include <ranges>
#include <vector>

template <typename Vertex, typename Edge>
class ListGraph {
public:
	using index_t = std::size_t;
	using vertex_t = Vertex;
	using edge_t = Edge;
	template <typename T>
	using storage_t = std::vector<T>;
	using size_t = std::size_t;

private:
	std::vector<Vertex> data;
	std::vector<std::vector<std::pair<index_t, edge_t>>> connections;
	size_t _size;

public:
	ListGraph(size_t size = 0, const Vertex& defaultV = Vertex()):
		data(size, defaultV), connections(size), _size(size) {}

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

	void resize(size_t size) {
		data.resize(size);
		connections.resize(size);
		_size = size;
	}

	void connect(index_t parent, index_t child, edge_t edge = edge_t()) {
		connections[parent].emplace_back(child, edge);
	}

	size_t size() const {
		return _size;
	}

	vertex_t& operator[](index_t index) {
		return data[index];
	}

	class child {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = child;

		child(ListGraph* graph, index_t parent, int list_index):
			graph(graph), parent(parent), list_index(list_index) {}

		child():
			graph(nullptr), list_index(0) {}

		index_t index() const {
			return graph->connections[parent][list_index].first;
		}
		edge_t& edge() {
			return graph->connections[parent][list_index].second;
		}
		edge_t const& edge() const {
			return graph->connections[parent][list_index].second;
		}

		vertex_t& value() {
			return graph->data[index()];
		}
		operator index_t() const {
			return index();
		}

		child operator*() const {
			return *this;
		}

		child& operator++() {
			++list_index;
			return *this;
		}
		
		void operator++(int) {
			++list_index;
		}

		bool operator!=(child const& other) const {
			return list_index != other.list_index;
		}

		bool operator==(child const& other) const {
			return list_index == other.list_index;
		}

	private:
		ListGraph* graph;
		index_t parent;
		int list_index;
	};

	auto children(index_t parent) {
		return std::ranges::subrange(
				child(this, parent, 0),
				child(this, parent, connections[parent].size())
				);
	}

	int degree(index_t parent) {
		return connections[parent].size();
	}

	void connect_both(index_t parent, index_t child, edge_t edge1 = edge_t(), edge_t edge2 = edge_t())
	requires std::same_as<int, decltype(edge_t().rev)> {
		edge1.rev = connections[child].size();
		edge2.rev = connections[parent].size();

		connect(parent, child, edge1);
		connect(child, parent, edge2);
	}

	child reverse(child original)
	requires std::same_as<int, decltype(edge_t().rev)> {
		return child(this, original.index(), original.edge().rev);
	}
};

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
		{child.value()} -> std::same_as<typename G::vertex_t&>;
		{child} -> std::convertible_to<typename G::index_t>;
	};
};

template <Graph G>
class TreeWrapper {
public:
	using index_t = G::index_t;
	using vertex_t = G::vertex_t;
	using edge_t = G::edge_t;
	template <typename T>
	using storage_t = typename G::template storage_t<T>;
	using size_t = G::size_t;

private:
	G& graph;
	index_t root;
	storage_t<index_t> parents;

public:
	TreeWrapper(G& graph, index_t root):
		graph(graph), root(root), parents(graph.size()) {

		std::queue<std::pair<index_t, index_t>> queue;
		queue.emplace(root, root);

		while (!queue.empty()) {
			auto [before_parent, parent] = queue.front();
			queue.pop();
			parents[parent] = before_parent;

			for (auto child: graph.children(parent)) {
				if (child == before_parent) continue;
				queue.emplace(parent, child);
			}
		}
	}

	vertex_t& operator[](index_t index) {
		return graph[index];
	}

	auto children(index_t parent) {
		return graph.children(parent) | std::views::filter([this, parent](auto it) {
				return it != parents[parent];
				});
	}

	int degree(index_t parent) {
		return graph.degree(parent) - (parent == root? 0 : 1);
	}

	index_t parent(index_t child) {
		return parents[child];
	}

	decltype(graph.size()) size() {
		return graph.size();
	}
};

template <typename T, T MOD>
struct StaticModPolicy {
	static_assert(MOD < std::numeric_limits<T>::max() / 2);
	static constexpr T mod() {
		return MOD;
	}
};

template <typename T, typename tag = void>
struct DynamicModPolicy {
	static T& mod() {
		static T value = 0;
		return value;
	}
};

template <typename T, typename T2, typename Policy>
class ModInt {
public:
	constexpr ModInt(T val) noexcept {
		if (val < 0) val += Policy::mod();
		val %= Policy::mod();
		value = val;
	}

	constexpr ModInt() noexcept:
		value(0) {}

private:
	T value;

	struct raw {};
	constexpr ModInt(T val, raw) noexcept:
		value(val) {}
public:
	constexpr static ModInt verified(T val) noexcept {
		return ModInt(val, raw{});
	}

	constexpr explicit operator T() const noexcept {
		return value;
	}

	constexpr ModInt operator+(ModInt const& other) const noexcept {
		if (value + other.value >= Policy::mod())
			return ModInt(value + other.value - Policy::mod(), raw{});
		else
		 	return ModInt(value + other.value, raw{});
	}

	constexpr ModInt& operator+=(ModInt const& other) noexcept {
		*this = *this + other;
		return *this;
	}

	constexpr ModInt operator*(ModInt const& other) const noexcept {
		return ModInt(static_cast<T2>(value) * other.value % Policy::mod(), raw{});
	}
};

template <uint32_t MOD>
using sm32 = ModInt<uint32_t, uint64_t, StaticModPolicy<uint32_t, MOD>>;

template <uint64_t MOD>
using sm64 = ModInt<uint64_t, uint64_t, StaticModPolicy<uint64_t, MOD>>;

using sm32_1e9_7 = sm32<1'000'000'007>;
using sm64_1e9_7 = sm64<1'000'000'007>;

template <typename tag = void>
using dm32 = ModInt<uint32_t, uint64_t, DynamicModPolicy<uint32_t, tag>>;

template <typename tag = void>
using dm64 = ModInt<uint64_t, uint64_t, DynamicModPolicy<uint64_t, tag>>;

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

sm32_1e9_7 traffic = 0;

template <Graph G>
void solve(G& tree, int root) {
	sm32_1e9_7 sum = 0;
	for (auto child: tree.children(root)) {
		solve(tree, child);
		traffic += tree[child] * child.edge() * sum;
		sum += tree[child] * child.edge();
	}
	traffic += sum;
	tree[root] = sum + 1;
}

int main() {
	FastIO();
	int n;
	std::cin >> n;

	ListGraph<sm32_1e9_7, sm32_1e9_7> graph(n);

	for (int i = 0; i < n - 1; i++) {
		int a, b, w;
		std::cin >> a >> b >> w;
		graph.connect(a - 1, b - 1, w);
		graph.connect(b - 1, a - 1, w);
	}

	TreeWrapper tree(graph, 0);

	solve(tree, 0);

	std::cout << (uint32_t)traffic;
}

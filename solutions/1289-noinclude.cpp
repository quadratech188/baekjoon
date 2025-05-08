#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <queue>
#include <ranges>
#include <type_traits>
#include <variant>
#include <vector>

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

	friend std::ostream& operator<<(std::ostream& os, ModInt const& val) {
		os << val.value;
		return os;
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
void solve(G& tree, size_t root) {
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
		graph.connect(a - 1, b - 1, sm32_1e9_7::verified(w));
		graph.connect(b - 1, a - 1, sm32_1e9_7::verified(w));
	}

	TreeWrapper tree(graph, 0);

	solve(tree, 0);

	std::cout << traffic;
}

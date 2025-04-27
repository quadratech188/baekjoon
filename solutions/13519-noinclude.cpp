#include <concepts>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <locale>
#include <queue>
#include <ranges>
#include <utility>
#include <variant>
#include <vector>

template <typename Vertex, typename Edge>
class ListGraph {
public:
	using index_t = int;
	using vertex_t = Vertex;
	using edge_t = Edge;
	template <typename T>
	using storage_t = std::vector<T>;
	using size_t = int;

private:
	std::vector<Vertex> data;
	std::vector<std::vector<std::pair<index_t, edge_t>>> connections;
	size_t _size;

public:
	ListGraph(size_t size, const Vertex& defaultV = Vertex()): data(size, defaultV), connections(size), _size(size) {}
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

struct Segment {
	constexpr Segment(): start(0), end(0) {}
	constexpr Segment(size_t start, size_t end): start(start), end(end) {}

	size_t start;
	size_t end;
	constexpr size_t size() const {
		return end - start;
	}
	constexpr size_t center() const {
		return (start + end) / 2;
	}
	constexpr Segment left() const {
		return Segment(start, center());
	}
	constexpr Segment right() const {
		return Segment(center(), end);
	}

	constexpr bool includes(const Segment& other) const {
		return start <= other.start && other.end <= end;
	}
};

template <typename T>
class DummyIterator {
public:
	DummyIterator(const T& val): _val(val) {}

	const T& operator*() const {
		return _val;
	}

	DummyIterator& operator++() {
		return *this;
	}
	void operator++(int) {}

private:
	const T& _val;
};

template <typename T>
concept Lazy = requires(T t, T l, T r) {
	{l + r} -> std::same_as<T>;
	{t.resolve(l, r)};
};

template<typename T> requires Lazy<T>
class LazySegmentTree {
public:
	LazySegmentTree(const size_t size, const T& val = T()):
	_size(size), _values(4 * size) {
		DummyIterator<T> iter(val);
		init(Segment(0, _size), 0, iter);
	}

	template <typename Iter>
	LazySegmentTree(const size_t size, Iter iterator):
		_size(size), _values(4 * size) {
		init(Segment(0, _size), 0, iterator);
	}

	template <std::ranges::range R>
	LazySegmentTree(const R& range):
		_size(std::ranges::size(range)), _values(4 * _size) {
		auto it = std::ranges::begin(range);
		init(Segment(0, _size), 0, it);
	}

	T sum(Segment segment) {
		return sum(segment, Segment(0, _size), 0);
	}

	T sum(size_t start, size_t end) {
		return sum(Segment(start, end));
	}

	T at(size_t index) {
		return sum(Segment(index, index + 1));
	}

	template <typename Callable>
	void update(Segment segment, Callable func) {
		return update(segment, 0, Segment(0, _size), func);
	}

	template <typename Callable>
	void update(size_t start, size_t end, Callable func) {
		return update(Segment(start, end), func);
	}

	template <typename Callable>
	void update(size_t index, Callable func) {
		return update(Segment(index, index + 1), func);
	}

	constexpr size_t size() const {
		return _size;
	}

	constexpr T const& root() {
		return this->_values[0];
	}

private:
	const size_t _size;
	std::vector<T> _values;

	template <typename Iter>
	void init(Segment segment, size_t index, Iter& iterator) {
		if (segment.size() == 1) {
			this->_values[index] = T(*iterator);
			++iterator;
			return;
		}

		size_t left = index * 2 + 1;
		size_t right = index * 2 + 2;

		init(segment.left(), left, iterator);
		init(segment.right(), right, iterator);

		_values[index] = _values[left] + _values[right];
	}

	T sum(Segment const query, Segment const segment, size_t const index) {
		if (query.includes(segment))
			return _values[index];

		size_t const left = index * 2 + 1;
		size_t const right = index * 2 + 2;

		_values[index].resolve(_values[left], _values[right]);

		if (segment.center() <= query.start)
			return sum(query, segment.right(), right);

		if (query.end <= segment.center())
			return sum(query, segment.left(), left);

		return sum(query, segment.left(), left)
		     + sum(query, segment.right(), right);
	}

	template <typename Callable>
	void update(Segment const index, size_t const value_index, Segment const segment, Callable func) {
		if (index.includes(segment)) {
			if constexpr (std::same_as<std::invoke_result_t<decltype(func), decltype(_values[value_index])>,
					bool>) {
				if (!std::invoke(func, _values[value_index]) || segment.size() == 1)
					return;
			} else {
				std::invoke(func, _values[value_index]);
				return;
			}
		}

		size_t const left = value_index * 2 + 1;
		size_t const right = value_index * 2 + 2;

		// Does the function mutate values?
		if constexpr (!std::invocable<Callable, const T&>)
			this->_values[value_index].resolve(this->_values[left], this->_values[right]);

		if (index.start < segment.center())
			update(index, left, segment.left(), func);

		if (segment.center() < index.end)
			update(index, right, segment.right(), func);

		if constexpr (!std::invocable<Callable, const T&>)
			_values[value_index] = _values[left] + _values[right];
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

template< typename T1, typename T2>
std::istream& operator>>(std::istream& is, std::pair<T1, T2>& data) {
	is >> data.first >> data.second;
	return is;
}

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& data) {
	for (auto& element: data)
		is >> element;
	return is;
}

namespace Input {
	template<typename T>
	inline std::vector<T> toVec(size_t n) {
		std::vector<T> result(n);
		std::cin >> result;

		return result;
	}
}

std::vector<int> subtree_sizes;
std::vector<int> heavy_edges;

template <Graph G>
int calculate_subtree_sizes(G& tree, int parent = 0) {
	int sum = 1;
	int max = 0;
	for (auto child: tree.children(parent)) {
		int size = calculate_subtree_sizes(tree, child);

		if (size > max) {
			max = size;
			heavy_edges[parent] = child;
		}

		sum += size;
	}

	subtree_sizes[parent] = sum;
	return sum;
}

std::vector<int> chain_heads;
std::vector<int> chain_parents;
std::vector<int> chain_indices;
std::vector<int> chain_depths;

template <Graph G>
void decompose(G& tree, int parent = 0, int depth = 0, int chain_head = 0, int chain_parent = 0) {
	static int index = 0;
	chain_heads[parent] = chain_head;
	chain_parents[parent] = chain_parent;

	chain_indices[parent] = index;
	index++;

	chain_depths[parent] = depth;

	if (heavy_edges[parent] == -1) return;

	decompose(tree, heavy_edges[parent], depth, chain_head, chain_parent);

	for (auto child: tree.children(parent)) {
		if (child != heavy_edges[parent]) {
			decompose(tree, child, depth + 1, child, parent);
		}
	}
}

struct Data {
	std::optional<int> update;

	int _sum_, _sum, sum_, sum;
	int length;

	Data(int val = 0):
		update(), _sum_(val), _sum(val), sum_(val), sum(val), length(1) {}
	Data(int _sum_, int _sum, int sum_, int sum, int length):
		update(), _sum_(_sum_), _sum(_sum), sum_(sum_), sum(sum), length(length) {}

	void fill(int val) {
		update = val;
		sum = val * length;
		if (val > 0) {
			_sum_ = val * length;
			_sum = val * length;
			sum_ = val * length;
		}
		else {
			_sum_ = 0;
			_sum = 0;
			sum_ = 0;
		}
	}

	Data operator+(Data const& other) const {
		return Data(
				std::max(std::max(_sum_, other._sum_), _sum + other.sum_),
				std::max(other._sum, _sum + other.sum),
				std::max(sum_, sum + other.sum_),
				sum + other.sum,
				length + other.length
				);
	}

	void resolve(Data& l, Data& r) {
		if (update.has_value()) {
			l.fill(*update);
			r.fill(*update);
			update.reset();
		}
	}

	Data reverse() {
		return Data(
				_sum_,
				sum_,
				_sum,
				sum,
				length
				);
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	ListGraph<std::monostate, std::monostate> graph(n);

	auto values = Input::toVec<int>(n);

	for (int i = 1; i < n; i++) {
		int a, b;
		std::cin >> a >> b;

		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
	}

	TreeWrapper tree(graph, 0);

	subtree_sizes.resize(n);
	heavy_edges.resize(n, -1);
	calculate_subtree_sizes(tree);

	chain_heads.resize(n);
	chain_parents.resize(n);
	chain_indices.resize(n);
	chain_depths.resize(n);
	decompose(tree);

	LazySegmentTree<Data> segtree(n);

	for (int i = 0; i < n; i++) {
		segtree.update(chain_indices[i], [&values, i](Data& val) {
				val.fill(values[i]);
				});
	}

	int m;
	std::cin >> m;
	for (int i = 0; i < m; i++) {
		char type;
		std::cin >> type;
		switch(type) {
			case '1': {
				int u, v;
				std::cin >> u >> v;
				u--;
				v--;

				Data u_sum, v_sum;

				while (chain_heads[u] != chain_heads[v]) {
					if (chain_depths[u] < chain_depths[v]) {
						v_sum = segtree.sum(
								chain_indices[chain_heads[v]],
								chain_indices[v] + 1
								) + v_sum;
						v = chain_parents[v];
					}
					else {
						u_sum = segtree.sum(
								chain_indices[chain_heads[u]],
								chain_indices[u] + 1
								) + u_sum;
						u = chain_parents[u];
					}
				}

				Data result;
				if (chain_indices[u] > chain_indices[v]) {
					result = u_sum.reverse() + segtree.sum(chain_indices[v], chain_indices[u] + 1).reverse() + v_sum;
				}
				else {
					result = u_sum.reverse() + segtree.sum(chain_indices[u], chain_indices[v] + 1) + v_sum;
				}

				std::cout << result._sum_ << '\n';
				break;
			}
			case '2': {
				int u, v, w;
				std::cin >> u >> v >> w;
				u--;
				v--;

				while (chain_heads[u] != chain_heads[v]) {
					if (chain_depths[u] < chain_depths[v]) std::swap(u, v);
					segtree.update(
							chain_indices[chain_heads[u]],
							chain_indices[u] + 1,
							[w](Data& val) {val.fill(w);});
					u = chain_parents[u];
				}

				if (chain_indices[u] > chain_indices[v])
					std::swap(u, v);

				segtree.update(
						chain_indices[u],
						chain_indices[v] + 1,
						[w](Data& val) {val.fill(w);});
				break;
			}
		}
	}
}

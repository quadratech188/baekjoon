#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
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

template <typename T, typename Operator = std::plus<T>>
class SegmentTree {
public:
	SegmentTree(const size_t size, const T& val = T(), Operator op = Operator()):
		_size(size), _values(4 * size), _operator(op) {
		init(Segment(0, _size), 0, DummyIterator<T>(val));
	}

	template <typename Iter>
	SegmentTree(const size_t size, Iter iterator, Operator op = Operator()):
		_size(size), _values(4 * size), _operator(op) {
		init(Segment(0, _size), 0, iterator);
	}

	template <std::ranges::range R>
	SegmentTree(R&& range, Operator op = Operator()):
		_size(std::ranges::distance(range)), _values(4 * _size), _operator(op) {
		init(Segment(0, _size), 0, std::ranges::begin(range)); 
	}

	inline T sum(Segment segment) {
		return sum(segment, Segment(0, _size), 0);
	}

	inline T sum(size_t start, size_t end) {
		return sum(Segment(start, end));
	}

	inline T root() {
		return sum(0, _size);
	}

	inline T at(size_t index) {
		return sum(Segment(index, index + 1));
	}

	template <typename Callable>
	void update(size_t index, Callable func) {
		return update(index, 0, Segment(0, _size), func);
	}

	inline size_t size() {
		return _size;
	}

#ifndef HACKABLE
private:
#endif
	const size_t _size;
	std::vector<T> _values;
	const Operator _operator;

	template <typename Iter>
	void init(Segment segment, size_t index, Iter&& iterator) {
		if (segment.size() == 1) {
			_values[index] = T(*iterator);
			++iterator;
			return;
		}

		size_t left = 2 * index + 1;
		size_t right = 2 * index + 2;
		init(segment.left(), left, iterator);
		init(segment.right(), right, iterator);

		_values[index] = _operator(_values[left], _values[right]);
	}

	T sum(Segment query, Segment segment, size_t index) {

		if (query.includes(segment))
			return _values[index];

		size_t left = 2 * index + 1;
		size_t right = 2 * index + 2;

		if (segment.center() <= query.start)
			return sum(query, segment.right(), right);

		if (query.end <= segment.center())
			return sum(query, segment.left(), left);

		return _operator(sum(query, segment.left(), left)
		          ,sum(query, segment.right(), right));
	}

	template <typename Callable>
	void update(size_t index, size_t value_index, Segment segment, Callable func) {
		if (segment.size() == 1) {
			std::invoke(func, _values[value_index]);
			return;
		}

		size_t left = 2 * value_index + 1;
		size_t right = 2 * value_index + 2;
		
		if (index < segment.center())
			update(index, left, segment.left(), func);
		else
		 	update(index, right, segment.right(), func);

		_values[value_index] = _operator(_values[left], _values[right]);
	}
};

template <typename T>
class Min {
public:
	T operator()(const T& l, const T& r) const {
		return std::min(l, r);
	}
};

template <typename T>
class Max {
public:
	T operator()(const T& l, const T& r) const {
		return std::max(l, r);
	}
};

template <typename T>
class Mul {
public:
	T operator()(const T& l, const T& r) const {
		return l * r;
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

int main() {
	FastIO();
	int n;
	std::cin >> n;

	ListGraph<std::monostate, std::monostate> graph(n);

	for (int i = 1; i < n; i++) {
		int u, v;
		std::cin >> u >> v;

		graph.connect(u - 1, v - 1);
		graph.connect(v - 1, u - 1);
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

	SegmentTree<int> segtree(n, 1);

	int m;
	std::cin >> m;
	for (int _ = 0; _ < n + m - 1; _++) {
		char type;
		std::cin >> type;
		switch(type) {
			case 'A': {
				int a, b;
				std::cin >> a >> b;
				a--;
				b--;

				segtree.update(tree.parent(b) == a? chain_indices[b]: chain_indices[a], [](int& val) {
						val = 0;
						});
				break;
		  	}
			case 'W': {
				int a;
				std::cin >> a;
				a--;

				int result = 0;
				while (chain_heads[a] != 0) {
					result += segtree.sum(chain_indices[chain_heads[a]], chain_indices[a] + 1);
					a = chain_parents[a];
				}

				if (a != 0)
					result += segtree.sum(1, chain_indices[a] + 1);

				std::cout << result << '\n';
			}
		}
	}
}

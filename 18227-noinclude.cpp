#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <ranges>
#include <utility>
#include <vector>

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

template <typename G>
class TreeWrapper {
public:
	using index_t = G::index_t;
	using vertex_t = G::vertex_t;
	using edge_t = G::edge_t;

private:
	G& graph;
	index_t root;
	std::vector<index_t> parents;

	void init(index_t parent, index_t before_parent) {
		parents[parent] = before_parent;
		for (auto it: graph.children(parent)) {
			if (it == before_parent) continue;
			init(it, parent);
		}
	}

public:
	TreeWrapper(G& graph, index_t root):
		graph(graph), root(root), parents(graph.size()) {
		init(root, root);
	}

	vertex_t& operator[](index_t index) {
		return graph[index];
	}

	auto children(index_t parent) {
		return graph.children(parent) | std::views::filter([this, parent](auto it) {
				return it != parents[parent];
				});
	}
};

struct None {};

struct Segment {
	Segment(): start(0), end(0) {}
	Segment(size_t start, size_t end): start(start), end(end) {}

	size_t start;
	size_t end;
	size_t size() {
		return end - start;
	}
	inline size_t center() {
		return (start + end) / 2;
	}
	inline Segment left() {
		return Segment(start, center());
	}
	inline Segment right() {
		return Segment(center(), end);
	}

	bool includes(const Segment& other) {
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
		_values(4 * size), _size(size), _operator(op) {
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

	T sum(Segment segment) {
		return sum(segment, Segment(0, _size), 0);
	}

	T sum(size_t start, size_t end) {
		return sum(Segment(start, end));
	}

	T root() {
		return sum(0, _size);
	}

	T at(size_t index) {
		return sum(Segment(index, index + 1));
	}

	template <typename Callable>
	void update(size_t index, Callable func) {
		return update(index, 0, Segment(0, _size), func);
	}

	size_t size() {
		return _size;
	}

private:
	std::vector<T> _values;
	const size_t _size;
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
			func(_values[value_index]);
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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

std::vector<Segment> segtree_indices;
std::vector<int> depths;
int segtree_index = 0;

template <typename G>
void calculate_indices(G&& graph, int parent, int before, int depth) {
	int prev_index = segtree_index;
	segtree_index++;
	for (auto child: graph.children(parent)) {
		if (child == before) continue;
		calculate_indices(graph, child, parent, depth + 1);
	}

	segtree_indices[parent] = Segment(prev_index, segtree_index);
	depths[parent] = depth;
}

int main() {
	FastIO();
	int n, c;
	std::cin >> n >> c;

	ListGraph<None, None> graph(n);

	for (int i = 0; i < n - 1; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
	}

	segtree_indices.resize(n);
	depths.resize(n);

	calculate_indices(graph, c - 1, c - 1, 1);

	SegmentTree<long long int> salaries(n);

	int q;
	std::cin >> q;

	for (int i = 0; i < q; i++) {
		int type, a;
		std::cin >> type >> a;

		switch(type) {
			case 1:
				salaries.update(segtree_indices[a - 1].start, [](long long int& val) {val++;});
				break;
			case 2:
				std::cout << salaries.sum(segtree_indices[a - 1]) * depths[a - 1] << '\n';
		}
	}
}

#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <locale>
#include <stack>
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

	class child_range {
	public:
		child_range(ListGraph* graph, index_t parent):
			graph(graph), parent(parent) {}
		child begin() const {
			return child(graph, parent, 0);
		}
		child end() const {
			return child(graph, parent, graph->connections[parent].size());
		}
	private:
		ListGraph* graph;
		index_t parent;
	};

	child_range children(index_t parent) {
		return child_range(this, parent);
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

struct None {};

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

	T sum(Segment query, Segment segment, size_t index) {
		if (query.includes(segment))
			return _values[index];

		size_t left = index * 2 + 1;
		size_t right = index * 2 + 2;

		_values[index].resolve(_values[left], _values[right]);

		if (segment.center() <= query.start)
			return sum(query, segment.right(), right);

		if (query.end <= segment.center())
			return sum(query, segment.left(), left);

		return sum(query, segment.left(), left)
		     + sum(query, segment.right(), right);
	}

	template <typename Callable>
	void update(Segment index, size_t value_index, Segment segment, Callable func) {
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

		size_t left = value_index * 2 + 1;
		size_t right = value_index * 2 + 2;

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

class LazyFill {
private:
	enum type {
		FILLED,
		UNFILLED,
		MIXED
	};

	LazyFill(type color, int filled, int length):
		color(color), filled(filled), length(length) {}

public:
	LazyFill():
		color(UNFILLED), filled(0), length(1) {}

	static LazyFill on() {
		return LazyFill(FILLED, 1, 1);
	}
	static LazyFill off() {
		return LazyFill(UNFILLED, 0, 1);
	}

private:
	type color;
	int filled;
	int length;

public:
	int count() {
		return filled;
	}

	void fill() {
		filled = length;
		color = FILLED;
	}
	void unfill() {
		filled = 0;
		color = UNFILLED;
	}

	void resolve(LazyFill& left, LazyFill& right) {
		switch(color) {
			case MIXED:
				return;
			case FILLED:
				left.fill();
				right.fill();
				break;
			case UNFILLED:
				left.unfill();
				right.unfill();
				break;
		}
		color = MIXED;
	}

	LazyFill operator+(const LazyFill& other) const {
		return LazyFill(MIXED, filled + other.filled, length + other.length);
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

std::vector<Segment> segtree_indices;
int segtree_index = 0;

void calculate_indices(ListGraph<int, None>& graph, int parent) {
	int prev_index = segtree_index;
	segtree_index++;
	for (auto child: graph.children(parent))
		calculate_indices(graph, child);

	segtree_indices[parent] = Segment(prev_index + 1, segtree_index);
}

int main() {
	FastIO();
	int n;
	std::cin >> n;

	ListGraph<int, None> graph(n);

	int garbage;
	std::cin >> garbage;

	for (int i = 1; i < n; i++) {
		int parent;
		std::cin >> parent;
		graph.connect(parent - 1, i);
	}

	segtree_indices.resize(n);

	calculate_indices(graph, 0);

	LazySegmentTree<LazyFill> tree(n, LazyFill::on());

	int m;
	std::cin >> m;

	for (int i = 0; i < m; i++) {
		int type, index;
		std::cin >> type >> index;

		Segment children = segtree_indices[index - 1];

		switch(type) {
			case 1:
				if (children.size() == 0) continue;
				tree.update(children,
						[](LazyFill& fill) {fill.fill();});
				break;
			case 2:
				if (children.size() == 0) continue;
				tree.update(children,
						[](LazyFill& fill) {fill.unfill();});
				break;
			case 3:
				std::cout << (children.size() == 0? 0: tree.sum(children).count()) << '\n';
		}
	}
}

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
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

struct None {};

template<typename T>
struct Vec2 {
	using type = T;
	
	T x, y;

	Vec2(T x, T y): x(x), y(y) {}
	Vec2(): x(T()), y(T()) {}

	Vec2 operator+(const Vec2& other) const {
		return Vec2(this->x + other.x, this->y + other.y);
	}

	Vec2 operator-(const Vec2& other) const {
		return Vec2(this->x - other.x, this->y - other.y);
	}

	Vec2& operator-=(Vec2 const& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<typename D>
	Vec2<D> operator/(const D other) const {
		return Vec2<D>(this->x / other, this->y / other);
	}

	Vec2 operator*(T const& other) const {
		return {x * other, y * other};
	}

	friend Vec2 operator*(T const& l, Vec2<T> const& r) {
		return {l * r.x, l * r.y};
	}

	bool operator<(const Vec2& other) const {
		if (this->x != other.x) return this->x < other.x;
		return this->y < other.y;
	}

	bool operator==(const Vec2& other) const {
		return this->x == other.x && this->y == other.y;
	}

	bool operator!=(const Vec2& other) const {
		return this->x != other.x || this->y != other.y;
	}

	Vec2 rotate(const double angle) const {
		return Vec2(
				x * std::cos(angle) - y * std::sin(angle),
				x * std::sin(angle) + y * std::cos(angle)
				);
	}

	T dot(const Vec2& other) const {
		return this->x * other.x + this->y * other.y;
	}

	T cross(const Vec2& other) const {
		this->x * other.y - this->y * other.x;
	}

	T size2() const {
		return this->x * this->x + this->y * this->y;
	}

	T length() const {
		return std::sqrt(size2());
	}

	double theta() const {
		return std::atan2(y, x);
	}

	static Vec2 zero() {
		return Vec2(0, 0);
	}

	static Vec2 one() {
		return Vec2(1, 1);
	}
};

template <typename T>
std::istream& operator>>(std::istream& is, Vec2<T>& vec2) {
	is >> vec2.x >> vec2.y;
	return is;
}

typedef Vec2<int> Int2;
typedef Vec2<double> Double2;

template <typename V2>
class Bounds2 {

public:
	Bounds2(V2 start, V2 end): left(start), right(end) {}
	Bounds2(V2 end): left(V2::zero()), right(end) {}

	struct iterator {
		using value_type = V2;
		using difference_type = std::ptrdiff_t;
		using pointer = const V2*;
		using reference = const V2&;
		using iterator_category = std::forward_iterator_tag;

		iterator(V2 pos, const Bounds2& bounds): pos(pos), bounds(bounds) {}
		V2 operator*() {
			return pos;
		}

		bool operator==(const iterator& other) {
			return pos == other.pos;
		}

		bool operator!=(const iterator& other) {
			return pos != other.pos;
		}

		iterator& operator++() {
			pos.x ++;
			if (pos.x == bounds.right.x) {
				pos.x = bounds.left.x;
				pos.y ++;
			}
			return *this;
		}
		
	private:
		V2 pos;
		const Bounds2& bounds;
	};

	iterator begin() const {
		return iterator(left, *this);
	}

	iterator end() const {
		return iterator({left.x, right.y}, *this); // so ugly
	}

	bool contains(V2 pos) const {
		return left.x <= pos.x && pos.x < right.x
			&& left.y <= pos.y && pos.y < right.y;
	}

	V2::type& x1() {
		return left.x;
	}
	V2::type& x2() {
		return right.x;
	}
	V2::type& y1() {
		return left.y;
	}
	V2::type& y2() {
		return right.y;
	}

private:
	V2 left, right;
};

template <typename T>
std::istream& operator>>(std::istream& is, Bounds2<T>& bounds2) {
	is >> bounds2.lt() >> bounds2.rd();
	return is;
}

typedef Bounds2<Int2> Range2;

template <typename T>
class Matrix {
public:

	Matrix(int columns, int rows, T defaultValue = T()):
		_values(columns * rows, defaultValue), _size(columns, rows) {}

	Matrix(): _size(Int2::zero()), _values() {}

	T& operator()(int column, int row) {
		return this->_values[row * _size.x + column];
	}

	const T& operator()(int column, int row) const {
		return this->_values[row * _size.x + column];
	}

	T& operator[](Int2 index) {
		return this->_values[index.y * _size.x + index.x];
	}

	bool operator==(const Matrix<T>& other) const {
		return _size == other._size && _values == other._values;
	}
	
	bool operator<(const Matrix<T>& other) const {
		if (_size != other._size) return _size < other._size;

		return _values < other._values;
	}

	Matrix operator*(const Matrix& other) {
		Matrix result(other._size.x, _size.y);

		for (Int2 index: result.bounds()) {
			for (int depth = 0; depth < _size.x; depth++) {
				result[index] += *this(depth, index.y) * *this(index.x, depth);
			}
		}

		return result;
	}

	Int2 size() {
		return _size;
	}

	Range2 bounds() {
		return Range2(Int2::zero(), _size);
	}

	int rawIndex(Int2 index) const {
		return index.y * _size.x + index.x;
	}

private:
	std::vector<T> _values;
	Int2 _size;
};

template<typename T>
std::istream& operator>>(std::istream& input, Matrix<T>& matrix) {
	for (Int2 index: matrix.bounds())
		input >> matrix[index];

	return input;
}

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

 // https://m.blog.naver.com/kks227/220810623254
 // wtf

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<None, None> graph(n + m + 2);
	Matrix<int> capacity(graph.size(), graph.size(), 0);
	Matrix<int> cost(graph.size(), graph.size(), 0);
	Matrix<int> flow(graph.size(), graph.size(), 0);

	int source = n + m;
	int sink = n + m + 1;

	for (int i = m; i < n + m; i++) {
		std::cin >> capacity(i, sink);
		graph.connect(sink, i);
		graph.connect(i, sink);
	}

	for (int i = 0; i < m; i++) {
		std::cin >> capacity(source, i);
		graph.connect(source, i);
		graph.connect(i, source);
	}

	for (int i = 0; i < m; i++) {
		for (int j = m; j < m + n; j++) {
			std::cin >> cost(i, j);
			cost(j, i) = - cost(i, j);
			capacity(i, j) = std::numeric_limits<int>::max();
			graph.connect(i, j);
			graph.connect(j, i);
		}
	}

	int result = 0;

	while (true) {
		std::vector<int> prev(graph.size(), -1);
		std::vector<int> accumulative_cost(graph.size(), std::numeric_limits<int>::max());
		std::vector<bool> in_queue(graph.size(), false);

		std::queue<int> queue;

		accumulative_cost[source] = 0;
		in_queue[source] = true;
		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			in_queue[parent] = false;
			for (auto child: graph.children(parent)) {
				if (capacity(parent, child) > flow(parent, child)
						&& accumulative_cost[child] > accumulative_cost[parent] + cost(parent, child)) {
					accumulative_cost[child] = accumulative_cost[parent] + cost(parent, child);
					prev[child] = parent;

					if (!in_queue[child]) {
						queue.push(child);
						in_queue[child] = true;
					}
				}
			}
		}

		if (prev[sink] == -1) break;

		int min_flow = std::numeric_limits<int>::max();
		for (int i = sink; i != source; i = prev[i])
			min_flow = std::min(min_flow, capacity(prev[i], i) - flow(prev[i], i));

		for (int i = sink; i != source; i = prev[i]) {
			result += min_flow * cost(prev[i], i);
			flow(prev[i], i) += min_flow;
			flow(i, prev[i]) -= min_flow;
		}
	}

	std::cout << result;
}

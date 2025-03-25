#include <cmath>
#include <cstdio>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <ranges>
#include <variant>
#include <vector>

template<typename T>
struct Vec2 {
	using type = T;
	
	T x, y;

	Vec2(T x, T y): x(x), y(y) {}
	Vec2(): x(T()), y(T()) {}

	Vec2 operator+(const Vec2& other) const {
		return Vec2(this->x + other.x, this->y + other.y);
	}

	Vec2& operator+=(Vec2 const& other) {
		x += other.x;
		y += other.y;
		return *this;
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

	T taxi_distance(const Vec2& other) const {
		return std::abs(x - other.x) + std::abs(y - other.y);
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
	using value_type = T;

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

	Int2 size() const {
		return _size;
	}

	Range2 bounds() const {
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

template <typename T>
std::ostream& operator<<(std::ostream& output, Matrix<T> const & matrix) {
	for (int j = 0; j < matrix.size().y; j++) {
		output << matrix(0, j);
		for (int i = 1; i < matrix.size().x; i++)
			output << '\t' << matrix(i, j);
		output << '\n';
	}
	return output;
}

template <typename Vertex, typename Edge>
class MatrixGraph {
public:
	using index_t = int;
	using vertex_t = Vertex;
	using edge_t = Edge;
	template <typename T>
	using storage_t = std::vector<T>;
	using size_t = int;

private:
	std::vector<Vertex> data;
	Matrix<Edge> connections;
	int _size;

public:
	MatrixGraph(int size, Vertex defaultV = Vertex()): data(size, defaultV), connections(size, size), _size(size) {}
	MatrixGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(values.size(), values.size()),
		_size(values.size()) {}

	Edge& edge(int parent, int child) {
		return connections(parent, child);
	}

	size_t size() {
		return _size;
	}

	Vertex& operator[](size_t index) {
		return data[index];
	}

	class child {
	public:
		child(MatrixGraph* graph, index_t parent, index_t self):
			graph(graph), parent(parent), self(self) {}

		child(): graph(nullptr), parent(), self() {}

		index_t index() {
			return self;
		}
		edge_t& edge() {
			return graph->edge(parent, self);
		}
		vertex_t& value() {
			return graph->data[self];
		}

		operator index_t() {
			return self;
		}

	private:
		MatrixGraph* graph;
		index_t parent;
		index_t self;
	};

	auto children(index_t parent) {
		return std::ranges::iota_view(0, _size)
			| std::views::filter([this, parent](index_t index) {
					return this->edge(parent, index);
					})
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

	MatrixGraph<std::monostate, int> graph(n);

	for (int i = 0; i < m; i++) {
		int a, b, t;
		std::cin >> a >> b >> t;
		graph.edge(a - 1, b - 1) = t;
		graph.edge(b - 1, a - 1) = t;
	}

	std::priority_queue<std::pair<int, int>> queue;
	std::vector<int> prev(n, -1);
	std::vector<int> times(n, std::numeric_limits<int>::max());

	queue.emplace(0, 0);
	times[0] = 0;

	while (!queue.empty()) {
		auto [time, parent] = queue.top();
		queue.pop();
		time *= -1;

		if (times[parent] < time) continue;

		if (parent == n - 1) break;

		for (auto child: graph.children(parent)) {
			if (time + child.edge() >= times[child]) continue;

			times[child] = time + child.edge();
			prev[child] = parent;
			queue.emplace(-times[child], child);
		}
	}

	if (prev[n - 1] == -1) {
		std::cout << -1;
		return 0;
	}

	int original = times[n - 1];

	std::vector<std::pair<int, int>> edges;
	for (int i = n - 1; i != 0; i = prev[i]) {
		edges.emplace_back(i, prev[i]);
	}

	int result = 0;

	for (auto edge: edges) {
		int temp = graph.edge(edge.first, edge.second);
		graph.edge(edge.first, edge.second) = 0;
		graph.edge(edge.second, edge.first) = 0;

		std::priority_queue<std::pair<int, int>> queue;
		std::vector<int> times(n, std::numeric_limits<int>::max());

		queue.emplace(0, 0);
		times[0] = 0;

		while (!queue.empty()) {
			auto [time, parent] = queue.top();
			queue.pop();
			time *= -1;

			if (times[parent] < time) continue;

			if (parent == n - 1) break;

			for (auto child: graph.children(parent)) {
				if (times[child] <= time + child.edge()) continue;

				times[child] = time + child.edge();
				queue.emplace(-times[child], child);
			}
		}

		result = std::max(result, times[n - 1]);

		graph.edge(edge.first, edge.second) = temp;
		graph.edge(edge.second, edge.first) = temp;
	}

	std::cout << (result == std::numeric_limits<int>::max()? -1: result - original);
}

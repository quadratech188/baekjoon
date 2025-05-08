#include <cmath>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <queue>
#include <ranges>
#include <type_traits>
#include <variant>
#include <vector>

template<typename T>
struct Vec2 {
	using type = T;
	
	T x, y;

	Vec2(T x, T y): x(x), y(y) {}
	Vec2(): x(T()), y(T()) {}

	Vec2 operator+(const Vec2& other) const noexcept {
		return Vec2(this->x + other.x, this->y + other.y);
	}

	Vec2& operator+=(Vec2 const& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2 operator-(const Vec2& other) const noexcept {
		return Vec2(this->x - other.x, this->y - other.y);
	}

	Vec2& operator-=(Vec2 const& other) noexcept {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<typename D>
	Vec2<D> operator/(const D other) const noexcept {
		return Vec2<D>(this->x / other, this->y / other);
	}

	Vec2 operator*(T const& other) const noexcept {
		return {x * other, y * other};
	}

	friend Vec2 operator*(T const& l, Vec2<T> const& r) noexcept {
		return {l * r.x, l * r.y};
	}

	bool operator<(const Vec2& other) const noexcept {
		if (this->x != other.x) return this->x < other.x;
		return this->y < other.y;
	}

	bool operator==(const Vec2& other) const noexcept {
		return this->x == other.x && this->y == other.y;
	}

	bool operator!=(const Vec2& other) const noexcept {
		return this->x != other.x || this->y != other.y;
	}

	Vec2 rotate(const double angle) const noexcept {
		return Vec2(
				x * std::cos(angle) - y * std::sin(angle),
				x * std::sin(angle) + y * std::cos(angle)
				);
	}

	T dot(const Vec2& other) const noexcept {
		return this->x * other.x + this->y * other.y;
	}

	T cross(const Vec2& other) const noexcept {
		return this->x * other.y - this->y * other.x;
	}

	T size2() const noexcept {
		return this->x * this->x + this->y * this->y;
	}

	double length() const noexcept {
		return std::sqrt(size2());
	}

	double theta() const noexcept {
		return std::atan2(y, x);
	}

	T taxi_distance(const Vec2& other) const noexcept {
		return std::abs(x - other.x) + std::abs(y - other.y);
	}

	static Vec2 zero() noexcept {
		return Vec2(0, 0);
	}

	static Vec2 one() noexcept {
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
	Bounds2(): left(V2::zero()), right(V2::zero()) {}

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

	template <typename T>
	friend std::istream& operator>>(std::istream& is, Bounds2<T>& bounds2) {
		is >> bounds2.left >> bounds2.right;
		return is;
	}

private:
	V2 left, right;
};

typedef Bounds2<Int2> Range2;

template <typename T>
class Matrix {
public:
	using value_type = T;

	Matrix(int columns, int rows, T defaultValue = T()):
		_values(columns * rows, defaultValue), _size(columns, rows) {}

	Matrix(): _size(Int2::zero()), _values() {}

	static Matrix identity(int n) {
		Matrix result(n, n);
		for (int i = 0; i < n; i++)
			result(i, i) = 1;

		return result;
	}

	Matrix identity() const {
		return identity(this->_size.x);
	}

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

	Matrix operator+(Matrix const& other) const {
		Matrix result(_size.x, _size.y);
		for (int i = 0; i < _values.size(); i++)
			result._values[i] = _values[i] + other._values[i];

		return result;
	}

	Matrix& operator+=(Matrix const& other) {
		for (int i = 0; i < _values.size(); i++)
			_values[i] += other._values[i];

		return *this;
	}

	Matrix& operator+=(T const& other) {
		for (auto& element: _values)
			element += other;

		return *this;
	}

	Matrix operator*(const Matrix& other) const {
		Matrix result(other._size.x, _size.y);

		for (Int2 index: result.bounds()) {
			T sum = T();
			for (int depth = 0; depth < _size.x; depth++) {
				sum += (*this)(depth, index.y) * other(index.x, depth);
			}
			result[index] = sum;
		}

		return result;
	}

	Int2 size() const {
		return _size;
	}

	Range2 bounds() const {
		return Range2(Int2::zero(), _size);
	}

	int rawIndex(Int2 const index) const {
		return index.y * _size.x + index.x;
	}

	T sum() const {
		return std::accumulate(_values.begin(), _values.end(), T());
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
			output << ' ' << matrix(i, j);
		output << '\n';
	}
	return output;
}

template <typename M, typename E = std::monostate>
class GridGraph {
public:
	using index_t = Int2;
	using vertex_t = M::value_type;
	using edge_t = E;
	template <typename T>
	using storage_t = Matrix<T>;
	using size_t = Int2;

	GridGraph(M& data, edge_t const& edge = edge_t()):
		data(data), edge(edge) {}
private:
	M& data;
	edge_t edge;
public:
	vertex_t& operator[](index_t index) {
		return data[index];
	}

	class child {
	public:
		child(GridGraph& graph, index_t index):
			graph(graph), _index(index) {}
	private:
		GridGraph& graph;
		index_t _index;
	public:
		index_t index() {
			return _index;
		}
		edge_t& edge() {
			return graph.edge;
		}
		vertex_t& value() {
			return graph[_index];
		}
		operator index_t() {
			return index();
		}
	};

	auto children(index_t parent) {
		static std::array<index_t, 4> deltas {
			index_t(1, 0),
			index_t(0, 1),
			index_t(-1, 0),
			index_t(0, -1)
		};

		return deltas | std::views::filter([this, parent](index_t delta) {
				return this->data.bounds().contains(parent + delta);
				}) | std::views::transform([this, parent](index_t delta) {
				return child(*this, parent + delta);
				});
	}

	size_t size() const {
		return data.size();
	}
};

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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

constexpr int lookup[6][6] = {
	10, 8, 7, 5, 0, 1,
	8, 6, 4, 3, 0, 1,
	7, 4, 3, 2, 0, 1,
	5, 3, 2, 2, 0, 1,
	0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0
};

struct Edge {
	int cost;
	int capacity;
};

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	Matrix<char> matrix(m, n);
	std::cin >> matrix;
	GridGraph<decltype(matrix)> graph(matrix);

	int const source = n * m;
	int const sink = n * m + 1;
	int const size = n * m + 2;
	ListGraph<std::monostate, Edge>::reversible<true> flowgraph(size);

	for (Int2 parent: matrix.bounds()) {
		int index = matrix.rawIndex(parent);

		flowgraph.connect_both(index, sink, {0, 1}, {0, 0});

		if ((parent.x + parent.y) % 2 != 0) continue;

		flowgraph.connect_both(source, index, {0, 1}, {0, 0});

		for (auto child: graph.children(parent)) {
			int child_index = matrix.rawIndex(child);

			int cost = lookup[matrix[parent] - 'A'][child.value() - 'A'];
			flowgraph.connect_both(index, child_index, {-cost, 1}, {cost, 0});
		}
	}

	int result = 0;
	int count = 0;

	std::vector<int> prev(size), dist(size);
	std::vector<decltype(flowgraph)::child*> edges(size);
	std::vector<bool> in_queue(size, false);
	std::queue<int> queue;

	while (true) {
		std::fill(prev.begin(), prev.end(), -1);
		std::fill(dist.begin(), dist.end(), std::numeric_limits<int>::max());

		dist[source] = 0;
		in_queue[source] = true;
		queue.push(source);

		while (!queue.empty()) {
			int parent = queue.front();
			queue.pop();
			in_queue[parent] = false;
			for (auto& child: flowgraph.children(parent)) {
				if (child.edge().capacity > 0
						&& dist[child] > dist[parent] + child.edge().cost) {
					dist[child] = dist[parent] + child.edge().cost;
					edges[child] = &child;
					prev[child] = parent;

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
			flowgraph.reverse(*edges[i]).edge().capacity += max_flow;
		}
		count++;
	}

	std::cout << - result;
}

#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <istream>
#include <type_traits>
#include <unistd.h>
#include <utility>
#include <vector>

#ifndef FASTISTREAM_BUFFER_SIZE
#define FASTISTREAM_BUFFER_SIZE 1 << 20
#endif

#ifndef FASTOSTREAM_BUFFER_SIZE
#define FASTOSTREAM_BUFFER_SIZE 1 << 20
#endif

namespace Fast {
	class istream {
	private:
		inline char getchar() {
			static char buffer[FASTISTREAM_BUFFER_SIZE];
			static char* ptr = buffer;
			static char* end = buffer;

			if (ptr == end) {
				ssize_t size = read(STDIN_FILENO, buffer, sizeof(buffer));
				if (size <= 0) return EOF;
				ptr = buffer;
				end = buffer + size;
			}
			return *(ptr++);
		}
	public:
		template <typename T>
		inline istream& operator>>(T& val)
		requires std::is_integral_v<T> {
			char ch;
			val = 0;

			do {
				ch = getchar();
			} while (std::isspace(ch));

			// Optimized away for non-signed types
			bool negative = false;
			if constexpr (std::is_signed_v<T>) {
				if (ch == '-') {
					negative = true;
					ch = getchar();
				}
			}

			do {
				val = 10 * val + ch - '0';
				ch = getchar();
			} while ('0' <= ch && ch <= '9');

			if constexpr (std::is_signed_v<T>)
				if (negative) val = -val;

			return *this;
		}

		inline istream& operator>>(char& val) {
			do {
				val = getchar();
			} while (std::isspace(val));
			return *this;
		}

		template <typename T1, typename T2>
		inline istream& operator>>(std::pair<T1, T2>& pair) {
			(*this) >> pair.first >> pair.second;
			return *this;
		}

		template <typename T>
		std::vector<T> to_vec(uint size) {
			std::vector<T> result(size);

			for (auto& val: result)
				(*this) >> val;

			return result;
		}
	};

	istream cin;
}
#ifdef HACKABLE
#define hackable_private public
#else
#define hackable_private private
#endif

#include <cstddef>
#include <type_traits>
#include <variant>
#include <vector>

template <typename V, typename E, bool Reversible = false, template <typename...> class Container = std::vector, typename Index = std::size_t>
class ListGraph {
public:
	// Builder
	template <bool value>
	using with_reversible = ListGraph<V, E, value, Container, Index>;
	template <template <typename...> class value>
	using with_container = ListGraph<V, E, Reversible, value, Index>;
	template <typename value>
	using with_index = ListGraph<V, E, Reversible, Container, value>;

	static constexpr bool reversible_v = Reversible;
	template <typename T>
	using container_t = Container<T>;

	using index_t = Index;
	using vertex_t = V;
	using edge_t = E;
	template <typename T>
	using storage_t = std::vector<T>;
	using size_t = std::size_t;

	class child {
		friend class ListGraph;

	public:
		child(index_t index, edge_t edge, index_t rev) noexcept:
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
	hackable_private:
		index_t _index;
		[[no_unique_address]]
		edge_t _edge;
		[[no_unique_address]]
		std::conditional_t<reversible_v, index_t, std::monostate> _rev;
	};

hackable_private:
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

	index_t add(vertex_t data = {}) {
		_data.push_back(data);
		_connections.emplace_back();
		_size ++;
		return _size - 1;
	}

	void reserve(size_t size) {
		_data.reserve(size);
		_connections.reserve(size);
	}

	void reserve_children(size_t size) {
		for (auto& connection: _connections)
			connection.reserve(size);
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

#include <functional>
#include <iostream>
#include <numeric>
#include <vector>


#include <cmath>

template<typename T, typename T2 = T>
struct Vec2 {
	using type = T;
	
	T x, y;

	constexpr Vec2(T x, T y) noexcept: x(x), y(y) {}
	constexpr Vec2() noexcept : x(T()), y(T()) {}

	constexpr Vec2 operator+(const Vec2& other) const noexcept {
		return Vec2(this->x + other.x, this->y + other.y);
	}

	constexpr Vec2& operator+=(Vec2 const& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}

	constexpr Vec2 operator-(const Vec2& other) const noexcept {
		return Vec2(this->x - other.x, this->y - other.y);
	}

	constexpr Vec2& operator-=(Vec2 const& other) noexcept {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<typename D>
	constexpr Vec2<D> operator/(const D other) const noexcept {
		return Vec2<D>(this->x / other, this->y / other);
	}

	constexpr Vec2 operator*(T const& other) const noexcept {
		return {x * other, y * other};
	}

	constexpr friend Vec2 operator*(T const& l, Vec2<T> const& r) noexcept {
		return {l * r.x, l * r.y};
	}

	constexpr bool operator<(const Vec2& other) const noexcept {
		if (this->x != other.x) return this->x < other.x;
		return this->y < other.y;
	}

	constexpr bool operator==(const Vec2& other) const noexcept {
		return this->x == other.x && this->y == other.y;
	}

	constexpr bool operator!=(const Vec2& other) const noexcept {
		return this->x != other.x || this->y != other.y;
	}

	constexpr Vec2 rotate(const double angle) const noexcept {
		return Vec2(
				x * std::cos(angle) - y * std::sin(angle),
				x * std::sin(angle) + y * std::cos(angle)
				);
	}

	constexpr T2 dot(const Vec2& other) const noexcept {
		return static_cast<T2>(this->x) * other.x
			+ static_cast<T2>(this->y) * other.y;
	}

	constexpr T2 cross(const Vec2& other) const noexcept {
		return static_cast<T2>(this->x) * other.y
			- static_cast<T2>(this->y) * other.x;
	}

	constexpr T2 size2() const noexcept {
		return static_cast<T2>(this->x) * this->x
			+ static_cast<T2>(this->y) * this->y;
	}

	constexpr auto length() const noexcept {
		return std::sqrt(size2());
	}

	constexpr auto theta() const noexcept {
		return std::atan2(y, x);
	}

	constexpr T taxi_distance(const Vec2& other) const noexcept {
		return std::abs(x - other.x) + std::abs(y - other.y);
	}

	constexpr static Vec2 zero() noexcept {
		return Vec2(0, 0);
	}

	constexpr static Vec2 one() noexcept {
		return Vec2(1, 1);
	}

	constexpr static Vec2 i() noexcept {
		return Vec2(1, 0);
	}

	constexpr static Vec2 j() noexcept {
		return Vec2(0, 1);
	}
};

template <typename IS, typename T, typename T2>
IS& operator>>(IS& is, Vec2<T, T2>& vec2) {
	is >> vec2.x >> vec2.y;
	return is;
}

template <typename OS, typename T, typename T2>
OS& operator<<(OS& os, Vec2<T, T2>& vec2) {
	os << vec2.x << ' ' << vec2.y;
	return os;
}

typedef Vec2<int32_t, int64_t> Int2;
typedef Vec2<double, double> Double2;

#include <cmath>
#include <iterator>

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

	Bounds2 pad(V2 start, V2 end) {
		return Bounds2(left + start, right - end);
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

	Matrix(): _values(), _size(Int2::zero()) {}

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

template<typename CIN, typename T>
CIN& operator>>(CIN& input, Matrix<T>& matrix) {
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
#include <algorithm>
#include <limits>
#include <queue>
#include <variant>

bool loop() {
	uint n, m;
	Fast::cin >> n >> m;

	if (n == 0 && m == 0) return false;

	uint s, d;
	Fast::cin >> s >> d;

	ListGraph<std::monostate, int> graph(n);

	for (uint i = 0; i < m; i++) {
		uint u, v;
		int p;
		Fast::cin >> u >> v >> p;

		graph.connect(u, v, p);
	}

	std::priority_queue<std::pair<int, uint>> queue;

	std::vector<int> distances(n, std::numeric_limits<int>::max());

	std::vector<std::vector<uint>> parents(n);

	distances[s] = 0;
	queue.emplace(0, s);

	while (!queue.empty()) {
		auto [distance, parent] = queue.top();
		queue.pop();
		distance = -distance;

		if (distance > distances[parent]) continue;

		for (auto const& child: graph.children(parent)) {
			if (distance + child.edge() > distances[child]) continue;

			if (distance + child.edge() == distances[child]) {
				parents[child].push_back(parent);
				continue;
			}

			parents[child].clear();
			parents[child].push_back(parent);

			distances[child] = distances[parent] + child.edge();
			queue.emplace(-distances[child], child);
		}
	}

	std::vector<bool> visited(n);
	std::queue<uint> queue2;

	Matrix<char> blacklist(n, n, false);

	visited[d] = true;
	queue2.push(d);

	while (!queue2.empty()) {
		uint child = queue2.front();
		queue2.pop();

		for (auto const& parent: parents[child]) {
			blacklist(parent, child) = true;

			if (visited[parent]) continue;
			visited[parent] = true;

			queue2.push(parent);
		}
	}

	std::ranges::fill(distances, std::numeric_limits<int>::max());

	distances[s] = 0;
	queue.emplace(0, s);

	while (!queue.empty()) {
		auto [distance, parent] = queue.top();
		queue.pop();
		distance = -distance;

		for (auto const& child: graph.children(parent)) {
			if (blacklist(parent, child)) continue;

			if (distance + child.edge() >= distances[child]) continue;

			distances[child] = distance + child.edge();
			queue.emplace(-distances[child], child);
		}
	}

	std::cout << (distances[d] == std::numeric_limits<int>::max()? -1: distances[d]) << '\n';

	return true;
}

int main() {
	while (loop()) {}
}

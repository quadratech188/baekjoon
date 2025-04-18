#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
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

class UnionFind {
public:
	UnionFind(int size) {
		this->size = size;
		data.reserve(size);
		for (int i = 0; i < size; i++)
			data.push_back(i);
	}

	int find(int x) {
		if (data[x] == x)
			return x;
		data[x] = find(data[x]);
		return data[x];
	}

	void connect(int x, int y) {
		data[find(x)] = find(y);
	}

	bool connected(int x, int y) {
		return find(x) == find(y);
	}

private:
	std::vector<int> data;
	int size;
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

static std::array<Int2, 4> deltas {
	Int2(1, 0),
	Int2(0, 1),
	Int2(-1, 0),
	Int2(0, -1)
};

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;
	Matrix<int> world(m, n);
	std::cin >> world;

	Matrix<int> groups(m, n, -1);
	std::vector<Int2> roots;

	for (Int2 root: world.bounds()) {
		if (world[root] == 0) continue;
		if (groups[root] != -1) continue;

		int index = roots.size();
		roots.push_back(root);

		std::queue<Int2> queue;
		queue.push(root);
		groups[root] = index;

		while (!queue.empty()) {
			Int2 parent = queue.front();
			queue.pop();

			for (Int2 delta: deltas) {
				Int2 child = parent + delta;
				if (!world.bounds().contains(child)) continue;
				if (world[child] == 0) continue;
				if (groups[child] != -1) continue;

				groups[child] = index;
				queue.push(child);
			}
		}
	}

	Matrix<int> distances(roots.size(), roots.size(), std::numeric_limits<int>::max());

	for (Int2 parent: world.bounds()) {
		if (groups[parent] == -1) continue;
		for (Int2 delta: deltas) {
			for (int i = 1;; i++) {
				Int2 child = parent + delta * i;
				if (!world.bounds().contains(child)) break;

				if (groups[child] == groups[parent]) break;

				if (groups[child] != -1) {
					if (i == 2) break;

					int& distance = distances(groups[parent], groups[child]);
					distance = std::min(distance, i - 1);
					break;
				}
			}
		}
	}

	std::vector<std::tuple<int, int, int>> edges;

	for (int i = 0; i < roots.size(); i++) {
		for (int j = i + 1; j < roots.size(); j++) {
			if (distances(i, j) != std::numeric_limits<int>::max())
				edges.emplace_back(distances(i, j), i, j);
		}
	}

	std::sort(edges.begin(), edges.end());

	UnionFind uf(roots.size());

	int total = 0;

	for (auto edge: edges) {
		if (uf.connected(std::get<1>(edge), std::get<2>(edge))) continue;

		uf.connect(std::get<1>(edge), std::get<2>(edge));
		total += std::get<0>(edge);
	}

	int parent = uf.find(0);
	for (int i = 1; i < roots.size(); i++) {
		if (parent != uf.find(i)) {
			std::cout << -1;
			return 0;
		}
	}

	std::cout << total;
}

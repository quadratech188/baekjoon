#include <cmath>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

template<typename T, typename T2 = T>
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

	T2 dot(const Vec2& other) const noexcept {
		return static_cast<T2>(this->x) * other.x
			+ static_cast<T2>(this->y) * other.y;
	}

	T2 cross(const Vec2& other) const noexcept {
		return static_cast<T2>(this->x) * other.y
			- static_cast<T2>(this->y) * other.x;
	}

	T2 size2() const noexcept {
		return static_cast<T2>(this->x) * this->x
			+ static_cast<T2>(this->y) * this->y;
	}

	auto length() const noexcept {
		return std::sqrt(size2());
	}

	auto theta() const noexcept {
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

template <typename T, typename T2>
std::istream& operator>>(std::istream& is, Vec2<T, T2>& vec2) {
	is >> vec2.x >> vec2.y;
	return is;
}

template <typename T, typename T2>
std::ostream& operator<<(std::ostream& os, Vec2<T, T2>& vec2) {
	os << vec2.x << ' ' << vec2.y;
	return os;
}

typedef Vec2<int32_t, int64_t> Int2;
typedef Vec2<double, double> Double2;

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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

Matrix<int> cache;

Matrix<char> room;

int solve(int prev_state, int row) {
	if (row == room.size().y) return 0;

	int& result = cache(prev_state, row);

	if (result != -1) return result;

	int const max = (1 << room.size().x) - 1;
	int blacklist = max;

	for (int i = 0; i < room.size().x; i++) {
		if (room(i, row) == 'x')
			blacklist &= (max - (1 << i));

		if (prev_state & (1 << i)) {
			if (i > 0)
				blacklist &= (max - (1 << (i - 1)));
			if (i < room.size().x - 1)
				blacklist &= (max - (1 << (i + 1)));
		}
	}

	for (int state = 0; state < (1 << room.size().x); state++) {
		if ((state | blacklist) != blacklist) continue;

		bool eligible = true;
		for (int i = 0; i < room.size().x - 1; i++) {
			if ((state & (1 << i)) && (state & (1 << (i + 1))))
				eligible = false;
		}
		if (!eligible) continue;

		int count = 0;
		for (int i = 0; i < room.size().x; i++) {
			if (state & (1 << i))
				count++;
		}

		result = std::max(result, count + solve(state, row + 1));
	}

	return result;
}

void loop() {
	int n, m;
	std::cin >> n >> m;

	room = Matrix<char>(m, n);

	cache = Matrix<int>(1 << m, n, -1);

	std::cin >> room;

	std::cout << solve(0, 0) << '\n';
}

int main() {
	FastIO();
	int c;
	std::cin >> c;
	for (int i = 0; i < c; i++) loop();
}

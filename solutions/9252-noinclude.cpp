#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

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

Matrix<int> cache;
Matrix<std::pair<int, int>> next;
std::string a, b;

int solve(uint a_ptr, uint b_ptr) {
	if (a_ptr == a.size() || b_ptr == b.size())
		return 0;

	if (cache(a_ptr, b_ptr) != -1)
		return cache(a_ptr, b_ptr);

	if (a[a_ptr] == b[b_ptr]) {
		next(a_ptr, b_ptr) = {a_ptr + 1, b_ptr + 1};
		return solve(a_ptr + 1, b_ptr + 1) + 1;
	}

	int l = solve(a_ptr, b_ptr + 1);
	int r = solve(a_ptr + 1, b_ptr);

	int result;
	if (l > r) {
		result = l;
		next(a_ptr, b_ptr) = {a_ptr, b_ptr + 1};
	}
	else {
		result = r;
		next(a_ptr, b_ptr) = {a_ptr + 1, b_ptr};
	}

	cache(a_ptr, b_ptr) = result;

	return result;
}

int main() {
	std::cin >> a >> b;

	cache = Matrix<int>(a.size(), b.size(), -1);
	next = Matrix<std::pair<int, int>>(a.size(), b.size());

	std::cout << solve(0, 0) << '\n';

	uint a_ptr = 0;
	uint b_ptr = 0;

	while (a_ptr != a.size() && b_ptr != b.size()) {
		if (a[a_ptr] == b[b_ptr])
			std::cout << a[a_ptr];

		auto temp = next(a_ptr, b_ptr);
		a_ptr = temp.first;
		b_ptr = temp.second;
	}
}

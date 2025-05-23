#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <iterator>
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

namespace Math {
	template<typename T>
	T power(T data, long long int exponent) {
		if (exponent == 1) return data;

		T half = power(data, exponent / 2);

		if (exponent % 2 == 0)
			return half * half;
		else
			return half * half * data;
	}

	template <typename T>
	T powerCeil(T power, T n) {
		T result = 1;

		while (result < n) result *= power;
		return result;
	}

	template <typename T>
	T factorial(T n) {
		T result = 1;
		for (T i = 1; i <= n; i++) result *= i;
		return result;
	}

	template <typename T>
	std::pair<T, T> quotient_remainder(T a, T b) {
		T quotient = a / b;
		T remainder = a % b;
		if (remainder < 0) {
			remainder += std::abs(b);
			quotient --;
		}
		return std::make_pair(quotient, remainder);
	}

	constexpr double pi = 3.1415926535897932384626;
	constexpr double tau = 2 * pi;
}

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

static std::array<Int2, 4> directions = {
	Int2(0, -1),
	Int2(0, 1),
	Int2(1, 0),
	Int2(-1, 0)
};

struct Shark {
	Shark(Int2 velocity, int size):
		velocity(velocity), size(size) {}
	Shark():
		velocity(0, 0), size(0) {}
	Int2 velocity;
	int size;
};

std::pair<Int2, Shark> bounce(Int2 pos, int x, int y, Shark shark) {
	auto [quotient_x, remainder_x] = Math::quotient_remainder(pos.x + shark.velocity.x, x);
	auto [quotient_y, remainder_y] = Math::quotient_remainder(pos.y + shark.velocity.y, y);

	Int2 new_pos;
	Shark result;
	result.size = shark.size;

	if (quotient_x % 2 == 0) {
		new_pos.x = remainder_x;
		result.velocity.x = shark.velocity.x;
	}
	else {
		new_pos.x = x - remainder_x;
		result.velocity.x = - shark.velocity.x;
	}

	if (quotient_y % 2 == 0) {
		new_pos.y = remainder_y;
		result.velocity.y = shark.velocity.y;
	}
	else {
		new_pos.y = y - remainder_y;
		result.velocity.y = - shark.velocity.y;
	}

	return std::make_pair(new_pos, result);
}

int main() {
	FastIO();
	int r, c, m;
	std::cin >> r >> c >> m;
	Matrix<Shark> sharks1(c, r);
	Matrix<Shark> sharks2(c, r);

	for (int i = 0; i < m; i++) {
		int r, c, s, d, z;
		std::cin >> r >> c >> s >> d >> z;
		sharks1(c - 1, r - 1) = Shark(directions[d - 1] * s, z);
	}

	decltype(sharks1)& primary = sharks1;
	decltype(sharks2)& secondary = sharks2;

	Range2 bounds(Int2(c - 1, r - 1));

	int sum = 0;

	for (int i = 0; i < c; i++) {
		for (int j = 0; j < r; j++) {
			if (primary(i, j).size != 0) {
				sum += primary(i, j).size;
				primary(i, j).size = 0;
				break;
			}
		}

		for (Int2 index: primary.bounds()) {
			if (primary[index].size == 0) continue;

			auto [new_pos, result] = bounce(index, c - 1, r - 1, primary[index]);

			Shark& target = secondary[new_pos];
			if (result.size > target.size)
				target = result;

			primary[index].size = 0;
		}

		std::swap(primary, secondary);
	}
	std::cout << sum;
}

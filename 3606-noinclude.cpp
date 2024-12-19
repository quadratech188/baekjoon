#include <cstdio>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <vector>

template<typename T, typename T2 = T>
struct Vec2 {
	T x, y;

	Vec2(T x, T y): x(x), y(y) {}
	Vec2(): x(T()), y(T()) {}

	Vec2 operator+(const Vec2& other) const {
		return Vec2(this->x + other.x, this->y + other.y);
	}

	Vec2 operator-(const Vec2& other) const {
		return Vec2(this->x - other.x, this->y - other.y);
	}

	template<typename D>
	Vec2 operator/(const D other) const {
		return Vec2(this->x / other, this->y / other);
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

	T2 dot(const Vec2& other) const {
		return static_cast<T2>(this->x) * other.x + static_cast<T2>(this->y) + other.y;
	}

	T2 cross(const Vec2& other) const {
		return static_cast<T2>(this->x) * other.y - static_cast<T2>(this->y) * other.x;
	}

	T2 size2() const {
		return static_cast<T2>(this->x) * this->x + static_cast<T2>(this->y) * this->y;
	}

	static Vec2 zero() {
		return Vec2(0, 0);
	}

	static Vec2 one() {
		return Vec2(1, 1);
	}
};

template <typename T, typename T2>
std::istream& operator>>(std::istream& is, Vec2<T, T2>& vec2) {
	is >> vec2.x >> vec2.y;
	return is;
}

typedef Vec2<int, long long int> Int2;

template <typename V2>
class Bounds {

public:
	Bounds(V2 start, V2 end): left(start), right(end) {}
	Bounds(V2 end): left(V2::zero()), right(end) {}

	struct iterator {
		using value_type = V2;
		using difference_type = std::ptrdiff_t;
		using pointer = const V2*;
		using reference = const V2&;
		using iterator_category = std::forward_iterator_tag;

		iterator(V2 pos, const Bounds& bounds): pos(pos), bounds(bounds) {}
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
		const Bounds& bounds;
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

private:
	V2 left, right;
};

typedef Bounds<Int2> Range2;

template <typename T>
class Matrix {
public:

	Matrix(int columns, int rows, T defaultValue = T()):
		_size(columns, rows), values(columns * rows, defaultValue) {}

	Matrix(): _size(Int2::zero()), values(std::vector<T>()) {}

	struct Column {
		Matrix& matrix;
		int column;

		Column(Matrix& matrix, int column): matrix(matrix), column(column) {}

		T& operator[](int row) {
			return matrix.at(column, row);
		}
		const T& operator[](int row) const {
			return matrix.at(column, row);
		}
	};

	T& at(int column, int row) {
		return this->values[row * _size.x + column];
	}

	const T& at(int column, int row) const {
		return this->values[row * _size.x + column];
	}

	Column operator[](int column) {
		return Column(*this, column);
	}

	T& operator[](Int2 index) {
		return at(index.x, index.y);
	}

	bool operator==(const Matrix<T>& other) const {
		return _size == other._size && values == other.values;
	}
	
	bool operator<(const Matrix<T>& other) const {
		if (_size != other._size) return _size < other._size;

		return values < other.values;
	}

	Matrix operator*(const Matrix& other) {
		Matrix result(other._size.x, _size.y);

		for (Int2 index: result.bounds()) {
			for (int depth = 0; depth < _size.x; depth++) {
				result[index] += at(depth, index.y) * other.at(index.x, depth);
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

	size_t hash() const {
		return std::hash<std::vector<T>>{}(values);
	}
	int rawIndex(Int2 index) const {
		return index.y * _size.x + index.x;
	}

private:
	std::vector<T> values;

	Int2 _size;
};

template<typename T>
std::istream& operator>>(std::istream& input, Matrix<T>& matrix) {
	for (Int2 index: matrix.bounds())
		input >> matrix[index];

	return input;
}

class ModInt {
public:
	ModInt(const int value, const int mod) {
		this->value = value % mod;
		if (this->value < 0) this->value += mod;
		this->mod = mod;
	}

	ModInt(const int mod): value(0), mod(mod) {}

	ModInt(): value(0), mod(0) {}

	ModInt operator+(const int& other) const {
		return ModInt(value + other, mod);
	}

	ModInt& operator++() {
		value = (value + 1) % mod;
		return *this;
	}

	ModInt operator++(int) {
		ModInt temp = *this;
		value = (value + 1) % mod;
		return temp;
	}

	ModInt& operator+=(const ModInt& other) {
		value = (value + other.value) % other.mod;
		mod = other.mod;
		return *this;
	}

	ModInt operator*(const ModInt& other) {
		return ModInt(value * other.value, other.mod);
	}

	void operator=(const int other) {
		value = other % mod;
		if (value < 0) value += mod;
	}

	bool operator<(const int other) {
		return value < other;
	}

	operator int() {
		return value;
	}

private:
	int value;
	int mod;
};

std::istream& operator>>(std::istream& is, ModInt& data) {
	int value;
	is >> value;
	data = value;

	return is;
}

namespace Math {
	template<typename T>
	T power(T& data, int exponent) {
		if (exponent == 1) return data;

		T half = power(data, exponent / 2);

		if (exponent % 2 == 0)
			return half * half;
		else
			return half * half * data;
	}
}

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

int main() {
	FastIO();
	int n, m, d, k;
	std::cin >> n >> m >> d >> k;

	Matrix<ModInt> initial(n, 1, ModInt(m));
	std::cin >> initial;

	Matrix<ModInt> multiplier(n, n, ModInt(m));

	for (int i = 0; i < n; i++) {
		for (int offset = -d; offset <= d; offset++) {
			multiplier[i][ModInt(i + offset, n)] = 1;
		}
	}

	Matrix<ModInt> result = initial * Math::power(multiplier, k);

	for (Int2 index: result.bounds())
		std::cout << result[index] << ' ';
}

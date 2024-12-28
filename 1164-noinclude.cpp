#include <algorithm>
#include <array>
#include <cstdio>
#include <functional>
#include <iostream>
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
	Vec2<D> operator/(const D other) const {
		return Vec2<D>(this->x / other, this->y / other);
	}

	Vec2<T2> operator*(const T other) const {
		return Vec2<T2>((T2)this->x * other, (T2)this->y * other);
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
		return static_cast<T2>(this->x) * other.x + static_cast<T2>(this->y) * other.y;
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

	Matrix(): _size(Int2::zero()), values() {}

	T& operator()(int column, int row) {
		return this->values[row * _size.x + column];
	}

	const T& operator()(int column, int row) const {
		return this->values[row * _size.x + column];
	}

	T& operator[](Int2 index) {
		return this->values[index.y * _size.x + index.x];
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
	std::vector<T> values;

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

std::array<Matrix<char>, 2> pieces;
std::array<int, 2> counts;

bool solution(Matrix<char>& grid, int count, int max) {
	if (count == max) return true;

	Int2 min_blank = grid.size();
	for (Int2 index: grid.bounds()) {
		if (grid[index] == 'X') {
			min_blank = index;
			break;
		}
	}
	
	for (int i = 0; i < 2; i++) {
		Matrix<char>& piece = pieces[i];
		if (min_blank.x + piece.size().x > grid.size().x || min_blank.y + piece.size().y > grid.size().y) continue;

		if (std::any_of(piece.bounds().begin(), piece.bounds().end(), [&piece, &grid, min_blank](Int2 index) {
					return piece[index] != 0 && grid[index + min_blank] != 'X';
					})) continue;

		for (Int2 index: piece.bounds()) {
			if (piece[index])
				grid[index + min_blank] = piece[index];
		}

		if (solution(grid, count + counts[i], max)) return true;

		for (Int2 index: piece.bounds()) {
			if (piece[index])
				grid[index + min_blank] = 'X';
		}
	}

	return false;
}

int main() {
	FastIO();
	
	pieces[0] = Matrix<char>(4, 2);
	pieces[0](0, 0) = 'A';
	pieces[0](0, 1) = 'A';
	pieces[0](1, 1) = 'A';
	pieces[0](2, 1) = 'A';
	pieces[0](3, 1) = 'A';
	pieces[0](3, 0) = 'A';

	pieces[1] = Matrix<char>(2, 1, 'B');

	counts[0] = 6;
	counts[1] = 2;

	int n, m;
	std::cin >> n >> m;
	Matrix<char> grid(m, n);

	std::cin >> grid;

	int count;
	for (Int2 index: grid.bounds())
		if (grid[index] == 'X') count++;

	if (solution(grid, 0, count)) {
		for (int y = 0; y < grid.size().y; y++) {
			for (int x = 0; x < grid.size().x; x++) {
				std::cout << grid(x, y);
			}
			std::cout << '\n';
		}
	}
	else {
		std::cout << -1;
	}
}

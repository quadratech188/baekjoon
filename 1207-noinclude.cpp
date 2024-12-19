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

template <typename T, typename T2 = T>
class Bounds2 {
	typedef Vec2<T, T2> V2;

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

private:
	V2 left, right;
};

typedef Bounds2<int, long long int> Range2;

template <typename T>
class Matrix {
public:

	Matrix(int columns, int rows, int defaultValue = T()):
		_size(columns, rows), values(columns * rows, defaultValue) {}

	Matrix(): _size(Int2::zero()), values(std::vector<T>()) {}

	T& at(int column, int row) {
		return this->values[row * _size.x + column];
	}

	struct Column {
		Matrix& matrix;
		int column;

		Column(Matrix& matrix, int column): matrix(matrix), column(column) {}

		T& operator[](int row) {
			return matrix.at(column, row);
		}
	};

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

	Int2 size() {
		return _size;
	}

	Range2 bounds() {
		return Range2(Int2::zero(), _size);
	}

	size_t hash() const {
		return std::hash<std::vector<T>>{}(values);
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

bool solution(Matrix<int>& square, std::vector<Matrix<char>>& pieces, std::array<bool, 5> used, int cnt) {

	Int2 first0 = square.size();

	for (Int2 t: square.bounds())
		if (square[t] == 0) {
			first0 = t;
			break;
		}

	if (first0 == square.size() && cnt == 5) return true;

	for (int i = 1; i <= 5; i++) {
		if (used[i - 1]) continue;

		Matrix<char>& piece = pieces[i - 1];

		int firstHash;

		for (int j = 0; j < piece.size().x; j++) {
			if (piece.at(j, 0) == '#') {
				firstHash = j;
				break;
			}
		}

		Int2 offset = first0;
		if (offset.x - firstHash < 0) continue;
		offset.x -= firstHash;

		if (offset.x + piece.size().x > square.size().x || offset.y + piece.size().y > square.size().y) continue;

		if (std::any_of(piece.bounds().begin(), piece.bounds().end(), [&square, &piece, offset](Int2 index) {
					return piece[index] == '#' && square[index + offset] != 0;
					})) continue;

		for (Int2 index: piece.bounds())
			if (piece[index] != '.') square[index + offset] = i;

		used[i - 1] = true;

		if (solution(square, pieces, used, cnt + 1)) return true;

		for (Int2 index: piece.bounds())
			if (piece[index] != '.') square[index + offset] = 0;

		used[i - 1] = false;
	}

	return false;
}

int main() {
	FastIO();
	int l;
	std::cin >> l;

	std::vector<Matrix<char>> pieces;
	pieces.reserve(5);

	for (int i = 0; i < 5; i++) {
		int n, m;
		std::cin >> n >> m;
		pieces.emplace_back(m, n);

		std::cin >> pieces[i];
	}

	Matrix<int> square(l, l);
	std::array<bool, 5> used {false, false, false, false, false};

	if (solution(square, pieces, used, 0)) {
		for (Int2 index: square.bounds()) {
			std::cout << square[index];

			if (index.x == square.size().x - 1)
				std::cout << '\n';
		}
	}
	else {
		std::cout << "gg";
	}
}

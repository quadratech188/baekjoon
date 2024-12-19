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

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;
	Matrix<char> grid(m, n);
	std::cin >> grid;

	UnionFind uf (m * n);

	for (Int2 index: grid.bounds()) {
		switch(grid[index]) {
			case 'D':
				uf.connect(grid.rawIndex(index), grid.rawIndex(index + Int2(0, 1)));
				break;
			case 'L':
				uf.connect(grid.rawIndex(index), grid.rawIndex(index + Int2(-1, 0)));
				break;
			case 'R':
				uf.connect(grid.rawIndex(index), grid.rawIndex(index + Int2(1, 0)));
				break;
			case 'U':
				uf.connect(grid.rawIndex(index), grid.rawIndex(index + Int2(0, -1)));
				break;
		}
	}
	std::vector<bool>used(m * n);
	for (Int2 index: grid.bounds()) {
		used[uf.find(grid.rawIndex(index))] = true;
	}

	int count = 0;
	for (bool use: used)
		if (use) count++;

	std::cout << count;
}

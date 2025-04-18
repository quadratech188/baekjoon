#include <algorithm>
#include <array>
#include <cstdio>
#include <functional>
#include <iostream>
#include <iterator>
#include <queue>
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

namespace Input {
	template<typename T>
	inline std::vector<T> inlineToVec(size_t n) {
		std::vector<T> result;
		result.reserve(n);
		for (int i = 0; i < n; i++) {
			T value;
			std::cin >> value;
			result.push_back(value);
		}
		return result;
	}
}

enum Direction {
	EW,
	NS
};

struct Light {
	int ew, ns;
	Direction initial;

	Direction state(int time) {
		time = (time - 1) % (ew + ns);

		if (initial == EW) {
			if (time < ew)
				return EW;
			else
			 	return NS;
		}
		else {
			if (time < ns)
				return NS;
			else
			 	return EW;
		}
	}
};

std::istream& operator>>(std::istream& is, Light& light) {
	char a;
	int i;

	std::cin >> i >> a >> light.ew >> light.ns;

	if (a == '-')
		light.initial = EW;
	else
	 	light.initial = NS;

	return is;
}

int main() {
	// FastIO();
	
	std::array<Int2, 4> directions {
		Int2(0, 1),
		Int2(1, 0),
		Int2(0, -1),
		Int2(-1, 0)
	};

	while (true) {
		int m, n;
		std::cin >> m >> n;

		if (m == 0 && n == 0) break;

		Matrix<char> grid(n, m);
		std::cin >> grid;

		Matrix<char> visited(n, m, false);

		std::queue<std::pair<Int2, int>> queue;

		int lightCount = 0;

		for (Int2 index: grid.bounds()) {
			switch(grid[index]) {
				case '#':
				case '.':
				case 'B':
					break;
				case 'A':
					visited[index] = true;
					queue.emplace(index, 0);
					break;
				default:
					lightCount = std::max(lightCount, grid[index] - '0' + 1);
			}
		}

		std::vector<Light> lights = Input::inlineToVec<Light>(lightCount);

		int result = 0;

		while (!queue.empty() && result == 0) {
			Int2 pos = queue.front().first;
			int time = queue.front().second;
			queue.pop();
			bool enQueued = false;

			for (Int2 direction: directions) {
				Int2 newPos = pos + direction;

				if (!grid.bounds().contains(newPos)) continue;
				if (visited[newPos]) continue;

				if (grid[newPos] == '.') continue;

				if (grid[newPos] == 'B') {
					result = time + 1;
					break;
				}

				if (grid[newPos] == '#') {
					visited[newPos] = true;
					queue.emplace(newPos, time + 1);
					continue;
				}

				Light& light = lights[grid[newPos] - '0'];

				Direction state = light.state(time + 1);

				if ((state == EW && direction.y == 0) || (state == NS && direction.x == 0)) {
					visited[newPos] = true;
					queue.emplace(newPos, time + 1);
				}
				else {
					if (!enQueued) {
						queue.emplace(pos, time + 1);
						enQueued = true;
					}
				}
			}
		}
		if (result == 0)
			std::cout << "impossible" << '\n';
		else
		 	std::cout << result << '\n';;
	}

	return 0;
}

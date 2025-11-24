#include <ranges>
#include <unistd.h>
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

		template <typename... Ts>
		inline istream& operator>>(std::tuple<Ts...>& tuple){
			std::apply([this](auto&... x) {(((*this) >> x), ...);}, tuple);
			return *this;
		}

		template <typename T>
		std::vector<T> to_vec(uint size) {
			std::vector<T> result(size);

			for (auto& val: result)
				(*this) >> val;

			return result;
		}

		template <typename T>
		auto to_range(uint size) {
			return std::views::iota(0u, size)
				| std::views::transform([this](uint) {
						T temp;
						(*this) >> temp;
						return temp;
						});
		}
	};

	istream cin;
}
#include <ranges>
#include <variant>

#include <ranges>
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

template <typename T>
class Matrix {
public:
	Matrix(int i, int j, T const& default_value = {}):
		Matrix({i, j}, default_value) {}

	Matrix(Int2 size, T const& default_value = {}):
		_size(size), _values(size.x * size.y, default_value) {}

	template <std::ranges::sized_range R>
	Matrix(int i, int j, R const& range):
		Matrix({i, j}, range) {}

	template <std::ranges::sized_range R>
	Matrix(Int2 size, R const& range):
		_size(size), _values(range.begin(), range.end()) {}
private:
	Int2 _size;
	std::vector<T> _values;

public:
	Int2 size() {
		return _size;
	}

	T& operator()(int i, int j) {
		return _values[_size.y * i + j];
	}
	T const& operator()(int i, int j) const {
		return _values[_size.y * i + j];
	}

	T& operator[](Int2 i) {
		return (*this)(i.x, i.y);
	}
	T const& operator[](Int2 i) const {
		return (*this)(i.x, i.y);
	}

	Matrix operator+(Matrix const& other) {
		Matrix result(_size);

		for (uint i = 0; i < _values.size(); i++) {
			result._values[i] = _values[i] + other._values[i];
		}
		return result;
	}

	Matrix operator*(Matrix const& other) {
		int depth = _size.y = other._size.x;

		Matrix result(_size.x, other._size.y, 0);

		for (int i = 0; i < result._size.x; i++) {
			for (int j = 0; j < result._size.y; j++) {
				for (int k = 0; k < depth; k++) {
					result(i, j) += (*this)(i, k) * other(k, j);
				}
			}
		}
		return result;
	}

	Matrix identity() {
		int size = _size.x = _size.y;
		Matrix result(size, size, 0);
		for (int i = 0; i < size; i++) {
			result(i, i) = 1;
		}
		return result;
	}

	template <typename IS>
	friend IS& operator>>(IS& is, Matrix& matrix) {
		for (auto& val: matrix._values)
			is >> val;
		return is;
	}

	template <typename OS>
	friend OS& operator<<(OS& os, Matrix const& matrix) {
		for (int i = 0; i < matrix._size.x; i++) {
			for (int j = 0; j < matrix._size.y; j++) {
				os << matrix(i, j) << ' ';
			}
			os << '\n';
		}
		return os;
	}
};

template <typename M, typename E = std::monostate>
class GridGraph {
public:
	using index_t = Int2;
	using vertex_t = M::value_type;
	using edge_t = E;
	template <typename T>
	using storage_t = Matrix<T>;
	using size_t = Int2;

	GridGraph(M& data, edge_t const& edge = edge_t()):
		data(data), edge(edge) {}
private:
	M& data;
	edge_t edge;
public:
	vertex_t& operator[](index_t index) {
		return data[index];
	}

	class child {
	public:
		child(GridGraph& graph, index_t index):
			graph(graph), _index(index) {}
	private:
		GridGraph& graph;
		index_t _index;
	public:
		index_t index() {
			return _index;
		}
		edge_t& edge() {
			return graph.edge;
		}
		vertex_t& value() {
			return graph[_index];
		}
		operator index_t() {
			return index();
		}
	};

	auto children(index_t parent) {
		static std::array<index_t, 4> deltas {
			index_t(1, 0),
			index_t(0, 1),
			index_t(-1, 0),
			index_t(0, -1)
		};

		return deltas | std::views::filter([this, parent](index_t delta) {
				return this->data.bounds().contains(parent + delta);
				}) | std::views::transform([this, parent](index_t delta) {
				return child(*this, parent + delta);
				});
	}

	size_t size() const {
		return data.size();
	}
};
#include <iostream>

Matrix<char> grid(0, 0);
Matrix<char> visited(0, 0);

int solve(Int2 pos, int remaining) {
	if (remaining == 0) {
		return pos == Int2(0, 0);
	}

	Int2 offsets[4] = {
		Int2(0, 1),
		Int2(1, 0),
		Int2(0, -1),
		Int2(-1, 0)
	};

	int result = 0;
	for (Int2 offset: offsets) {
		Int2 new_pos = pos + offset;
		if (new_pos.x < 0 || grid.size().x <= new_pos.x) continue;
		if (new_pos.y < 0 || grid.size().y <= new_pos.y) continue;

		if (grid[new_pos] == 'T') continue;
		if (visited[new_pos]) continue;

		visited[new_pos] = true;
		result += solve(new_pos, remaining - 1);
		visited[new_pos] = false;
	}
	return result;
}

int main() {
	int r, c;
	int k;
	Fast::cin >> r >> c >> k;
	grid = Matrix<char>(r, c);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			Fast::cin >> grid(r - 1 - i, j);
		}
	}
	visited = Matrix<char>(r, c, false);

	visited(r - 1, c - 1) = true;
	std::cout << solve(Int2(r - 1, c - 1), k - 1);
}

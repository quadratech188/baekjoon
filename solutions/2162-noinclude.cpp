#include <algorithm>


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
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <istream>
#include <type_traits>
#include <unistd.h>
#include <utility>
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

		template <typename T>
		std::vector<T> to_vec(uint size) {
			std::vector<T> result(size);

			for (auto& val: result)
				(*this) >> val;

			return result;
		}
	};

	istream cin;
}
#ifdef HACKABLE
#define hackable_private public
#else
#define hackable_private private
#endif

#include <numeric>
#include <vector>

class UnionFind {
public:
	UnionFind(size_t size) {
		next.resize(size);
		std::iota(next.begin(), next.end(), 0);
	}

	size_t find(size_t x) {
		if (next[x] == x)
			return x;
		next[x] = find(next[x]);
		return next[x];
	}

	void connect(size_t from, size_t to) {
		next[find(from)] = find(to);
	}

	bool connected(size_t x, size_t y) {
		return find(x) == find(y);
	}

hackable_private:
	std::vector<size_t> next;
};

bool check_frac(int64_t numerator, int64_t denominator) {
	if (denominator == 0) return true;

	if (denominator < 0)
		return denominator <= numerator && numerator <= 0;
	else
	 	return 0 <= numerator && numerator <= denominator;
}

bool intersects(Int2 a1, Int2 a2, Int2 b1, Int2 b2) {

	bool result = true;

	// a1 + (a2 - a1) x = b1 + (b2 - b1) y
	
	// (a2 - a1) cross (b2 - b1) x = (b1 - a1) cross (b2 - b1)
	
	result &= check_frac((b1 - a1).cross(b2 - b1), (a2 - a1).cross(b2 - b1));

	// (a1 - b1) cross (a2 - a1) = (b2 - b1) cross (a2 - a1) y
	result &= check_frac((a1 - b1).cross(a2 - a1), (b2 - b1).cross(a2 - a1));

	if ((a2 - a1).cross(b2 - b1) == 0) {
		result &= (b1 - a1).cross(a2 - a1) == 0;

		if (a1.x > a2.x) std::swap(a1, a2);
		if (b1.x > b2.x) std::swap(b1, b2);
		result &= a1.x <= b2.x && b1.x <= a2.x;

		if (a1.y > a2.y) std::swap(a1, a2);
		if (b1.y > b2.y) std::swap(b1, b2);
		result &= a1.y <= b2.y && b1.y <= a2.y;
	}

	return result;
}

int main() {
	uint n;
	Fast::cin >> n;

	auto lines = Fast::cin.to_vec<std::pair<Int2, Int2>>(n);
	UnionFind uf(n);

	for (uint i = 0; i < n; i++) {
		for (uint j = i + 1; j < n; j++) {
			if (intersects(lines[i].first, lines[i].second, lines[j].first, lines[j].second))
				uf.connect(i, j);
		}
	}
	std::vector<uint> counts(n);
	for (uint i = 0; i < n; i++)
		counts[uf.find(i)] ++;

	std::cout << std::ranges::count_if(counts, [](uint val) {return val != 0;})
		<< '\n' << std::ranges::max(counts);
}

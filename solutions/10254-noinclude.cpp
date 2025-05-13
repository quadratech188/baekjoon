#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <stack>
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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

template <typename T, T MOD>
struct StaticModPolicy {
	static_assert(MOD < std::numeric_limits<T>::max() / 2);
	static constexpr T mod() {
		return MOD;
	}
};

template <typename T, typename tag = void>
struct DynamicModPolicy {
	static T& mod() {
		static T value = 0;
		return value;
	}
};

template <typename T, typename T2, typename Policy>
class ModInt {
public:
	constexpr ModInt(T val) noexcept {
		if (val < 0) val += Policy::mod();
		val %= Policy::mod();
		value = val;
	}

	constexpr ModInt() noexcept:
		value(0) {}

private:
	T value;

	struct raw {};
	constexpr ModInt(T val, raw) noexcept:
		value(val) {}
public:
	constexpr static ModInt verified(T val) noexcept {
		return ModInt(val, raw{});
	}

	constexpr T val() const noexcept {
		return value;
	}

	constexpr explicit operator T() const noexcept {
		return value;
	}

	constexpr ModInt operator+(ModInt const& other) const noexcept {
		if (value + other.value >= Policy::mod())
			return ModInt(value + other.value - Policy::mod(), raw{});
		else
		 	return ModInt(value + other.value, raw{});
	}

	constexpr ModInt& operator+=(ModInt const& other) noexcept {
		*this = *this + other;
		return *this;
	}

	constexpr ModInt& operator++() noexcept {
		if (value == Policy::mod() - 1)
			value = 0;
		else
		 	++value;
		return *this;
	}

	constexpr ModInt operator*(ModInt const& other) const noexcept {
		return ModInt(static_cast<T2>(value) * other.value % Policy::mod(), raw{});
	}

	constexpr bool operator!=(T const& other) const noexcept {
		return value != other;
	}

	friend std::ostream& operator<<(std::ostream& os, ModInt const& val) {
		os << val.value;
		return os;
	}

	static void set_mod(T val) {
		Policy::mod() = val;
	}
};

template <uint32_t MOD>
using sm32 = ModInt<uint32_t, uint64_t, StaticModPolicy<uint32_t, MOD>>;

template <uint64_t MOD>
using sm64 = ModInt<uint64_t, uint64_t, StaticModPolicy<uint64_t, MOD>>;

using sm32_1e9_7 = sm32<1'000'000'007>;
using sm64_1e9_7 = sm64<1'000'000'007>;

template <typename tag = void>
using dm32 = ModInt<uint32_t, uint64_t, DynamicModPolicy<uint32_t, tag>>;

template <typename tag = void>
using dm64 = ModInt<uint64_t, uint64_t, DynamicModPolicy<uint64_t, tag>>;

void loop() {
	size_t n;
	std::cin >> n;

	std::vector<Int2> points(n);
	for (auto& point: points)
		std::cin >> point;

	size_t min_index = 0;

	for (size_t i = 1; i < n; i++) {
		if (points[i] < points[min_index])
			min_index = i;
	}

	std::swap(points[0], points[min_index]);

	Int2 min = points[0];

	std::sort(std::next(points.begin()), points.end(), [min](Int2 const& l, Int2 const& r) {
			if ((l - min).cross(r - min) != 0) return (l - min).cross(r - min) > 0;
			return (l - min).size2() < (r - min).size2();
			});

	std::vector<Int2> shell;

	for (auto& point: points) {
		while (shell.size() > 1 && (shell[shell.size() - 1] - shell[shell.size() - 2])
					.cross(point - shell[shell.size() - 1]) <= 0) {
			shell.pop_back();
				}
		shell.push_back(point);
	}
	dm32<>::set_mod(shell.size());

	dm32<> a = shell.size() - 1;
	dm32<> b = 0;
	dm32<> c = 0;
	dm32<> d = 1;

	size_t city1, city2;
	int64_t length_square = 0;

	size_t count = 0;
	while (count != shell.size()) {
		int64_t new_length_square = (shell[c.val()] - shell[a.val()]).size2();
		if (length_square < new_length_square) {
			length_square = new_length_square;
			city2 = c.val();
			city1 = a.val();
		}
		if ((shell[b.val()] - shell[a.val()]).cross(shell[d.val()] - shell[c.val()]) > 0) {
			++c;
			++d;
		}
		else {
			++a;
			++b;
			++count;
		}
	}

	std::cout << shell[city1] << ' ' << shell[city2] << '\n';
}

int main() {
	FastIO();
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) loop();
}

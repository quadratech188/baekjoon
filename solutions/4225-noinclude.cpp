#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <stack>
#include <vector>

template<typename T>
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

	T dot(const Vec2& other) const noexcept {
		return this->x * other.x + this->y * other.y;
	}

	T cross(const Vec2& other) const noexcept {
		return this->x * other.y - this->y * other.x;
	}

	T size2() const noexcept {
		return this->x * this->x + this->y * this->y;
	}

	double length() const noexcept {
		return std::sqrt(size2());
	}

	double theta() const noexcept {
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

template <typename T>
std::istream& operator>>(std::istream& is, Vec2<T>& vec2) {
	is >> vec2.x >> vec2.y;
	return is;
}

typedef Vec2<int> Int2;
typedef Vec2<double> Double2;

template< typename T1, typename T2>
std::istream& operator>>(std::istream& is, std::pair<T1, T2>& data) {
	is >> data.first >> data.second;
	return is;
}

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& data) {
	for (auto& element: data)
		is >> element;
	return is;
}

namespace Input {
	template<typename T>
	inline std::vector<T> toVec(size_t n) {
		std::vector<T> result(n);
		std::cin >> result;

		return result;
	}
}

int count = 1;

bool loop() {
	int n;
	std::cin >> n;

	if (n == 0) return false;

	std::vector<Int2> points = Input::toVec<Int2>(n);

	auto min = std::min_element(points.begin(), points.end());
	Int2 min_p = *min;

	std::swap(points[0], *min);
	std::sort(points.begin() + 1, points.end(), [&min](Int2& lhs, Int2& rhs) {
			int cross = (lhs - *min).cross(rhs - *min);
			if (cross != 0)
				return cross > 0;

			if (lhs.x != rhs.x)
				return lhs.x < rhs.x;

			return lhs.y < rhs.y;
			});

	std::vector<Int2> shell;

	for (int i = 0; i < points.size(); i++) {
		while (shell.size() >= 2 &&
				(shell[shell.size() - 1] - shell[shell.size() - 2])
				.cross(points[i] - shell[shell.size() - 1])
				<= 0) {
			shell.pop_back();
		}
		shell.push_back(points[i]);
	}

	double diameter = std::numeric_limits<double>::max();

	for (int i = 0; i < shell.size(); i++) {
		Int2 direction = shell[(i + 1) % shell.size()] - shell[i];

		double max = 0;

		for (auto& point: shell) {
			max = std::max(max,
					direction.cross(point - shell[i]) / direction.length());
		}
		diameter = std::min(diameter, max);
	}

	std::cout << "Case " << count << ": " << std::ceil(diameter * 100) / 100 << '\n';
	count++;

	return true;
}

int main() {
	std::cout << std::fixed;
	std::cout.precision(2);

	while (loop()) {}
}

#pragma once

#include <cmath>
#include <cstdint>
#include <iostream>

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

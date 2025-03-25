#pragma once

#include <cmath>
#include <iostream>

template<typename T>
struct Vec2 {
	using type = T;
	
	T x, y;

	Vec2(T x, T y): x(x), y(y) {}
	Vec2(): x(T()), y(T()) {}

	Vec2 operator+(const Vec2& other) const {
		return Vec2(this->x + other.x, this->y + other.y);
	}

	Vec2& operator+=(Vec2 const& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2 operator-(const Vec2& other) const {
		return Vec2(this->x - other.x, this->y - other.y);
	}

	Vec2& operator-=(Vec2 const& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<typename D>
	Vec2<D> operator/(const D other) const {
		return Vec2<D>(this->x / other, this->y / other);
	}

	Vec2 operator*(T const& other) const {
		return {x * other, y * other};
	}

	friend Vec2 operator*(T const& l, Vec2<T> const& r) {
		return {l * r.x, l * r.y};
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

	Vec2 rotate(const double angle) const {
		return Vec2(
				x * std::cos(angle) - y * std::sin(angle),
				x * std::sin(angle) + y * std::cos(angle)
				);
	}

	T dot(const Vec2& other) const {
		return this->x * other.x + this->y * other.y;
	}

	T cross(const Vec2& other) const {
		this->x * other.y - this->y * other.x;
	}

	T size2() const {
		return this->x * this->x + this->y * this->y;
	}

	T length() const {
		return std::sqrt(size2());
	}

	double theta() const {
		return std::atan2(y, x);
	}

	T taxi_distance(const Vec2& other) const {
		return std::abs(x - other.x) + std::abs(y - other.y);
	}

	static Vec2 zero() {
		return Vec2(0, 0);
	}

	static Vec2 one() {
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

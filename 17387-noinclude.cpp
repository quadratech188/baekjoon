#include <iostream>

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

typedef Vec2<double, double> Double2;

bool intersects(Double2 v1, Double2 v2, Double2 v3, Double2 v4) {
	double ratio1 = (v2 - v1).cross(v3 - v1);
	double ratio2 = (v2 - v1).cross(v4 - v1);

	if (ratio1 == 0 && ratio2 == 0) {
		return 0 <= (v3 - v1).dot(v2 - v1) &&
			(v3 - v1).dot(v2 - v1) <= (v2 - v1).size2() ||
			0 <= (v4 - v1).dot(v2 - v1) &&
			(v4 - v1).dot(v2 - v1) <= (v2 - v1).size2() ||
			intersects(v3, v4, v1, v2);
	}

	if (ratio1 * ratio2 > 0) return false;

	Double2 intersection = (v3 * ratio2 - v4 * ratio1) / (ratio2 - ratio1);

	return 0 <= (intersection - v1).dot(v2 - v1) &&
		(intersection - v1).dot(v2 - v1) <= (v2 - v1).size2();
}

int main() {
	Double2 v1, v2, v3, v4;
	std::cin >> v1 >> v2 >> v3 >> v4;

	std::cout << intersects(v1, v2, v3, v4);
}

#include <algorithm>
#include <cstdio>
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
};

template <typename T, typename T2>
std::istream& operator>>(std::istream& is, Vec2<T, T2>& vec2) {
	is >> vec2.x >> vec2.y;
	return is;
}

typedef Vec2<int, long long int> Int2;

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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

struct Edge {
	Int2 center2;
	long long int length2;
	Int2 edge;

	Edge(Int2 a, Int2 b) {
		center2 = a + b;
		edge = b - a;
		length2 = edge.size2();
	}

	bool operator<(const Edge& other) const {
		if (this->center2 != other.center2) return this->center2 < other.center2;
		if (this->length2 != other.length2) return this->length2 < other.length2;
		return this->edge < other.edge;
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	std::vector<Int2> points = Input::inlineToVec<Int2>(n);

	std::vector<Edge> edges;
	edges.reserve(n * (n - 1) / 2);

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			edges.emplace_back(points[i], points[j]);
		}
	}

	std::sort(edges.begin(), edges.end());

	long long int max = 0;

	for (auto current = edges.begin(); current != edges.end(); current++) {
		for (auto next = std::next(current); next != edges.end(); next++) {
			if (next->center2 != current->center2 || next->length2 != current->length2) break;

			max = std::max(max, current->edge.cross(next->edge) / 2);
		}
	}

	std::cout << max;
}

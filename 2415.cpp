#include "modules/Vec2.h"
#include "modules/Input.h"
#include "modules/FastIO.h"
#include <algorithm>
#include <iostream>
#include <iterator>

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

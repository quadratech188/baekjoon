#include "CodeForces.h"
#include "FastIO2.h"
#include "Matrix.h"
#include "LazySegmentTree.h"
#include <algorithm>
#include <iostream>
#include <limits>

struct Element {
	int value;
	int lazy;

	Element(int x = std::numeric_limits<int>::max()):
		value(x), lazy(std::numeric_limits<int>::max()) {}

	int get() const {
		return std::min(value, lazy);
	}

	void apply() {
		value = get();
	}

	Element operator+(Element const& other) const {
		return Element(std::min(get(), other.get()));
	}

	void propagate(Element& l, Element& r) {
		l.lazy = std::min(l.lazy, lazy);
		r.lazy = std::min(r.lazy, lazy);
	}
	
	void reinit(Element const& l, Element const& r) {
		value = std::min(l.value, r.value);
		lazy = std::numeric_limits<int>::max();
	}
};

/*
 * rows [a, b]:
 * we only care about sequential edges
 *
 * S = (b - a) * (col[i] - col[i - 1])
 *
 * scan?
 * figure out next col: total m
 * fill in: total m log n (lazy seg)
 *
 * total: n * n * m log n
 *
 * maybe???
 */

void loop() {
	int n, m;
	Fast::cin >> n >> m;

	Matrix<char> grid(0, 0);

	bool rotated = n > m;
	if (rotated) {
		std::swap(n, m);

		grid = Matrix<char>(n, m);
		for (int j = 0; j < m; j++) {
			for (int i = 0; i < n; i++) {
				Fast::cin >> grid(i, j);
			}
		}
	}
	else {
		grid = Matrix<char>(n, m, Fast::cin.to_range<char>(n * m));
	}

	Matrix<int> last_edge(n, n, -1);

	std::vector<LazySegmentTree<Element>> trees(m, LazySegmentTree<Element>(n));

	for (int col = 0; col < m; col++) {
		for (int l = 0; l < n; l++) {
			for (int r = l + 1; r < n; r++) {
				if (grid(l, col) == '0' || grid(r, col) == '0') continue;

				int prev_col = last_edge(l, r);
				last_edge(l, r) = col;
				
				if (prev_col == -1) continue;

				int size = (r - l + 1) * (col - prev_col + 1);

				for (int i = prev_col; i <= col; i++) {
					trees[i].update(l, r + 1, [size](Element& e) {
							e.lazy = std::min(e.lazy, size);
							});
				}
			}
		}
	}
	Matrix<int> result(n, m);
	for (int j = 0; j < m; j++) {
		for (int i = 0; i < n; i++) {
			int x = trees[j].at(i).get();
			result(i, j) = x == std::numeric_limits<int>::max()? 0: x;
		}
	}
	if (rotated) {
		for (int j = 0; j < m; j++) {
			for (int i = 0; i < n; i++) {
				std::cout << result(i, j) << ' ';
			}
			std::cout << '\n';
		}
	}
	else {
		std::cout << result;
	}
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}

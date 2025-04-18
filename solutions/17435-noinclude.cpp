#include <cmath>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>

template <typename T>
struct Matrix {
	std::vector<T> values;

	int sizeX, sizeY;

	Matrix(int x, int y, int defaultValue = T()) {
		values = std::vector<T>(x * y);
		sizeX = x;
		sizeY = y;
	}

	T& at(int x, int y) {
		return this->values[x * sizeY + y];
	}
	Matrix(): sizeX(0), sizeY(0), values(std::vector<T>()) {}
};

struct SparseTable {
	Matrix<size_t> values;

	SparseTable(size_t n, int iterations, std::function<int(int)> func) {
		int sum = 0;

		size_t depth = 1;
		while (true) {
			sum += (1 << (depth - 1));
			if (sum >= iterations) break;
			depth ++;
		}

		values = Matrix<size_t>(n, depth);

		for (size_t i = 0; i < n; i++) {
			values.at(i, 0) = func(i);
		}

		for (size_t row = 1; row < depth; row++) {
			for (size_t i = 0; i < n; i++) {
				size_t intermediate = values.at(i, row - 1);
				values.at(i, row) = values.at(intermediate, row - 1);
			}
		}
	}

	size_t after(size_t index, int iterations) {
		int i = 0;
		size_t result = index;

		for (size_t i = 0; iterations > 0; i++) {
			if (iterations & 1) {
				result = values.at(result, i);
			}
			iterations >>= 1;
		}

		return result;
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

int main() {
	FastIO();
	int m;
	std::cin >> m;

	std::vector<int> lookup;
	lookup.reserve(m);
	for (int i = 0; i < m; i++) {
		int temp;
		std::cin >> temp;
		lookup.push_back(temp - 1);
	}

	SparseTable table(m, 500000, [&lookup](size_t index) {
			return lookup[index];
			});

	int q;
	std::cin >> q;

	for (int i = 0; i < q; i++) {
		int n, x;
		std::cin >> n >> x;
		std::cout << table.after(x - 1, n) + 1 << '\n';
	}
}

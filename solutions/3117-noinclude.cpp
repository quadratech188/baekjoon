#include <cmath>
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

int main() {
	int n, k, m;
	std::cin >> n >> k >> m;

	std::vector<int> initial;
	initial.reserve(n);

	for (int i = 0; i < n; i++) {
		int temp;
		std::cin >> temp;
		initial.push_back(temp - 1);
	}

	std::vector<int> lookup;
	lookup.reserve(n);

	for (int i = 0; i < k; i++) {
		int temp;
		std::cin >> temp;
		lookup.push_back(temp - 1);
	}

	SparseTable table(k, m, [&lookup](size_t n) {
			return lookup[n];
			});

	for (auto a: initial) {
		std::cout << table.after(a, m - 1) + 1 << ' ';
	}
}

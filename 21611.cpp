#include "modules/Math.h"
#include "modules/FastIO.h"
#include "modules/Matrix.h"
#include <iostream>
int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;
	Matrix<int> indices(n, n, -1);

	Int2 center((n - 1) / 2, (n - 1) / 2);

	Int2 pos = center;
	Int2 dir(-1, 0);

	for (int i = 0; i < n * n; i++) {
		indices[pos] = i;

		pos += dir;
		if (indices[pos + dir.rotate(- Math::pi / 2)] == -1)
			dir = dir.rotate(- Math::pi / 2);
	}

	std::vector<int> values1, values2;
	std::vector<int>& primary = values1;
	std::vector<int>& secondary = values2;

	values1.resize(n * n, 0);
	for (Int2 index: indices.bounds()) {
		std::cin >> primary[indices[index]];
	}
	primary[0] = -1;

	std::array<int, 3> counts {0};

	for (int i = 0; i < m; i++) {
		int d, s;
		std::cin >> d >> s;

		static std::array<Int2, 4> directions {
			Int2(0, -1),
			Int2(0, 1),
			Int2(-1, 0),
			Int2(1, 0)
		};

		for (int j = 1; j <= s; j++)
			primary[indices[center + j * directions[d - 1]]] = 0;

		secondary.resize(0);
		for (auto value: primary) {
			if (value != 0) secondary.push_back(value);
		}
		std::swap(primary, secondary);

		bool updated;
		do {
			secondary.resize(1, -1);
			updated = false;
			int left = 1;
			while (left != primary.size()) {
				int right = left;
				while (right < primary.size() && primary[right] == primary[left]) right++;
				if (right - left >= 4) {
					counts[primary[left] - 1] += right - left;
					updated = true;
				}
				else {
					for (int i = left; i < right; i++)
						secondary.push_back(primary[i]);
				}
				left = right;
			}
			std::swap(primary, secondary);
		} while (updated);

		secondary.resize(1, -1);
		int left = 1;
		while (left != primary.size()) {
			int right = left;
			while (right < primary.size() && primary[right] == primary[left]) right++;
			secondary.push_back(right - left);
			secondary.push_back(primary[left]);
			left = right;
		}
		std::swap(primary, secondary);

		secondary.resize(n * n, 0);
		for (int i = 0; i < std::min(n * n, (int)primary.size()); i++)
			secondary[i] = primary[i];
		std::swap(primary, secondary);
	}

	std::cout << counts[0] + 2 * counts[1] + 3 * counts[2];
}

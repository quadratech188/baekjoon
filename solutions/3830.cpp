#include <iostream>
#include <numeric>
#include <vector>
#include <cstdio>

std::vector<int> parents;
std::vector<int> weights;

int find(int child) {
	if (parents[child] == child) return child;

	int prev = parents[child];
	parents[child] = find(parents[child]);
	weights[child] += weights[prev];

	return parents[child];
}

bool loop() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	int n, m;
	std::cin >> n >> m;
	if (n == 0 && m == 0) return false;

	parents.resize(n);
	std::iota(parents.begin(), parents.end(), 0);
	weights.assign(n, 0);

	for (int i = 0; i < m; i++) {
		char type;
		std::cin >> type;

		switch (type) {
			case '!': {
				int a, b, w;
				std::cin >> a >> b >> w;
				int a_parent = find(a - 1);
				int b_parent = find(b - 1);

				weights[b_parent] = weights[a_parent] + weights[a - 1] - weights[b - 1] + w;
				parents[b_parent] = a_parent;

				break;
			}
			case '?': {
				int a, b;
				std::cin >> a >> b;
				if (find(a - 1) != find(b - 1))
					std::cout << "UNKNOWN\n";
				else
				 	std::cout << weights[b - 1] - weights[a - 1] << '\n';
		    }
		}
	}

	return true;
}

int main() {
	while (loop()) {}
}

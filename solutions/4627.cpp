#include "modules/Matrix.h"
#include "modules/Types.h"
#include "modules/ListGraph.h"
#include <iostream>

void loop() {
	int n, d;
	std::cin >> n >> d;
	std::string line;
	std::cin >> line;
	int m = line.size();
	Matrix<char> capacity(m, n);
	for (int i = 0; i < m; i++)
		capacity(i, 0) = line[i];

	for (Int2 index: Range2(Int2(0, 1), Int2(m, n)))
		std::cin >> capacity[index];

	std::vector<Int2> locations;
	for (Int2 index: capacity.bounds()) {
		char state;
		std::cin >> state;
		if (state == 'L') locations.push_back(index);
	}

	ListGraph<None, None> graph(capacity.size().x * capacity.size().y);

	for (auto i: locations) {
		for (auto j: locations) {
			if (i == j) continue;
			graph.connect(capacity.rawIndex(i), capacity.rawIndex(j));
		}
	}
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) loop();
}

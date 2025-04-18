#include "modules/ListGraph.h"
#include "modules/Types.h"
#include <algorithm>
#include <iostream>
#include <array>

int main() {
	int n;
	std::cin >> n;
	ListGraph<None> graph(n);

	for (int i = 0; i < n - 1; i++) {
		int a, b;
		std::cin >> a >> b;
		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
	}
}

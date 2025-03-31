#include "modules/ListGraph.h"
#include <iostream>
#include <variant>

int main() {
	int n, m;
	std::cin >> n >> m;

	ListGraph<std::monostate, std::monostate> graph(2 * n);

	auto not_ = [n](int i) {return i + n;};

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		if (a < 0) a = n - a;
		if (b < 0) b = n - b;
		a--;
		b--;
		graph.connect(a, b);
		graph.connect(b, a);
	}
}

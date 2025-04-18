#include "modules/Graph.h"
#include <iostream>
#include <unordered_map>

int required(Graph<int>::Node root) {
	int results = 0;

	root.forEachChild([](Graph<int>::Node child) {

			});
}

int main() {
	int n;
	std::cin >> n;

	Graph<int> tree(n);

	for (int i = 0; i < n; i++) {
		int v, a, d;

		std::cin >> v >> a >> d;

		tree.value(v - 1) = a;

		for (int j = 0; j < d; j++) {
			int child;
			std::cin >> child;
			tree.link(v - 1, child);
		}
	}
}

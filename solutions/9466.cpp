#include "../modules/FastIO2.h"
#include "../modules/ListGraph2.h"
#include <queue>
#include <variant>

void loop() {
	uint n;
	Fast::cin >> n;

	ListGraph<std::monostate, std::monostate> graph(n);

	for (uint i = 0; i < n; i++) {
		uint child;
		Fast::cin >> child;

		graph.connect(i, child - 1);
	}

	std::vector<uint> degree(n);

	for (uint i = 0; i < n; i++) {
		for (auto const& child: graph.children(i))
			degree[child] ++;
	}

	std::queue<uint> queue;

	for (uint i = 0; i < n; i++) {
		if (degree[i] == 0)
			queue.push(i);
	}

	uint cnt = 0;
	while (!queue.empty()) {
		uint parent = queue.front();
		queue.pop();

		cnt++;

		for (auto const& child: graph.children(parent)) {
			degree[child] --;
			if (degree[child] == 0)
				queue.push(child);
		}
	}

	std::cout << cnt << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++)
		loop();
}

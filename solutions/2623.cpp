#include "../modules/ListGraph2.h"
#include "../modules/FastIO2.h"
#include <queue>
#include <variant>

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	ListGraph<std::monostate, std::monostate> graph(n);

	for (uint i = 0; i < m; i++) {
		uint cnt;
		Fast::cin >> cnt;

		uint prev, current;
		Fast::cin >> prev;
		prev --;

		for (uint j = 1; j < cnt; j++) {
			Fast::cin >> current;
			current --;
			graph.connect(prev, current);
			prev = current;
		}
	}

	std::vector<int> degree(n);

	for (uint i = 0; i < n; i++) {
		for (auto const& child: graph.children(i))
			degree[child]++;
	}

	std::queue<uint> queue;

	for (uint i = 0; i < n; i++) {
		if (degree[i] == 0)
			queue.push(i);
	}

	std::vector<uint> result;
	result.reserve(n);

	while (!queue.empty()) {
		uint parent = queue.front();
		queue.pop();

		result.push_back(parent);

		for (auto const& child: graph.children(parent)) {
			degree[child]--;

			if (degree[child] == 0)
				queue.push(child);
		}
	}

	if (result.size() != n)
		std::cout << '0';
	else {
		for (auto const& val: result)
			std::cout << val + 1 << ' ';
	}
}

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
int main() {
	int n;
	std::cin >> n;

	std::vector<std::vector<int>> graph(n);
	std::vector<int> lens(n), depends(n), finishes(n);

	std::queue<int> queue;

	for (int i = 0; i < n; i++) {
		std::cin >> lens[i] >> depends[i];
		if (depends[i] == 0) {
			queue.push(i);
			finishes[i] = lens[i];
		}
		for (int k = 0; k < depends[i]; k++) {
			int o;
			std::cin >> o;
			o --;
			graph[o].push_back(i);
		}
	}

	while (!queue.empty()) {
		int p = queue.front();
		queue.pop();

		for (int c: graph[p]) {
			depends[c] --;
			finishes[c] = std::max(finishes[c], finishes[p] + lens[c]);
			if (depends[c] == 0) {
				queue.push(c);
			}
		}
	}

	int max = 0;
	for (auto x: finishes) {
		max = std::max(max, x);
	}

	std::cout << max << '\n';
}

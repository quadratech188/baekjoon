#include <algorithm>
#include <iostream>
#include <queue>

#include "modules/Input.h"
#include "modules/ListGraph.h"
#include "modules/GraphAlgs.h"
#include "modules/FastIO.h"

enum kdh {
	K,
	D,
	H
};

struct KDH {
	
	kdh ch;

	bool isParent(const KDH other) {
		switch(ch) {
			case K: return other.ch == D;
			case D: return other.ch == H;
			case H: return other.ch == K;
			default: return false;
		};
	}
};

std::istream& operator>>(std::istream& is, KDH& obj) {
	char temp;
	std::cin >> temp;
	switch(temp) {
		case 'K': obj.ch = K; break;
		case 'D': obj.ch = D; break;
		case 'H': obj.ch = H; break;
	}
	return is;
}

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	ListGraph<KDH> graph(Input::inlineToVec<KDH>(n));

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		a --;
		b --;

		if (graph[a].isParent(graph[b]))
			graph.connect(a, b);
		if (graph[b].isParent(graph[a]))
			graph.connect(b, a);
	}

	std::vector<int> dependencies = GraphAlgs::dependencies(graph);

	std::queue<int> queue;

	std::vector<int> depth(n, 0);

	for (int i = 0; i < n; i++) {
		if (dependencies[i] == 0) {
			queue.push(i);
			switch(graph[i].ch) {
				case K: depth[i] = 1; break;
				case D: depth[i] = -1; break;
				case H: depth[i] = 0; break;
			}
		}
	}

	if (queue.empty()) {
		std::cout << -1;
		return 0;
	}

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		graph.forEachChild(parent, [&queue, &dependencies, &depth, parent](int child, KDH& value) {
				depth[child] = std::max(depth[child], depth[parent] + 1);
				dependencies[child] --;
				if (dependencies[child] == 0) {
					queue.push(child);
				}
				});
	}

	for (int i = 0; i < n; i++) {
		if (dependencies[i] > 0) {
			std::cout << -1;
			return 0;
		}
	}

	int max = *std::max_element(depth.begin(), depth.end());

	if (max < 3)
		std::cout << -1;
	else
		std::cout << (max / 3) * 3;
}

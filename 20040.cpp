#include "modules/UnionFind.h"
#include "modules/FastIO.h"
#include <iostream>

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	UnionFind uf(n);

	int result = 0;

	for (int i = 1; i <= m; i++) {
		int a, b;
		std::cin >> a >> b;
		if (result != 0) continue;
		
		if (uf.connected(a, b)) {
			result = i;
			continue;
		}

		uf.connect(a, b);
	}

	std::cout << result;
}

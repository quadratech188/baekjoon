#include "../modules/FastIO2.h"
#include "../modules/UnionFind.h"

int main() {
	uint g, p;
	Fast::cin >> g >> p;

	auto gs = Fast::cin.to_vec<uint>(p);

	UnionFind uf(g + 1);

	uint cnt = 0;
	for (auto const& g_i: gs) {
		uint loc = uf.find(g_i);
		if (loc == 0) break;

		uf.connect(loc, loc - 1);
		cnt++;
	}

	std::cout << cnt;
}

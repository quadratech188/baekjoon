#include "../modules/FastIO2.h"
#include "../modules/UnionFind.h"
#include <algorithm>

int main() {
	uint n, m, k;
	Fast::cin >> n >> m >> k;

	auto a = Fast::cin.to_vec<uint>(m);
	std::ranges::sort(a);

	UnionFind uf(m + 1);

	for (uint i = 0; i < k; i++) {
		uint b;
		Fast::cin >> b;

		uint index = uf.find(std::ranges::upper_bound(a, b) - a.begin());
		uf.connect(index, index + 1);

		std::cout << a[index] << '\n';
	}
}

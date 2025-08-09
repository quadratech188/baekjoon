#include "../modules/FastIO2.h"
#include "../modules/Compress.h"
#include "../modules/SegmentTree2.h"

int main() {
	uint n;
	Fast::cin >> n;

	auto as = Fast::cin.to_vec<int>(n);
	auto bs = Fast::cin.to_vec<int>(n);

	Compress<int> compress(as);

	SegmentTree<uint> tree(n);

	int64_t result = 0;
	for (auto val: bs) {
		result += tree.sum(compress.compress(val), n);
		tree.update(compress.compress(val), [](uint& ch) {ch = 1;});
	}
	std::cout << result;
}

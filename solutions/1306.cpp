#include "../modules/FastIO.h"
#include "../modules/FastIO2.h"
#include "../modules/OverloadedPlus.h"
#include "../modules/SegmentTree2.h"
#include "../modules/InputRange.h"

int main() {
	FastIO();
	size_t n, m;
	Fast::cin >> n >> m;

	SegmentTree<maxxer<int>> tree(InputRange<int>(n, Fast::cin));

	for (size_t i = m - 1; i <= n - m; i++)
		std::cout << tree.sum(i - m + 1, i + m).val() << ' ';
}

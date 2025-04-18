#include <iostream>
#include "modules/FastIO.h"
#include "modules/SegmentTree.h"
#include "modules/InputIterator.h"

struct Data {
	Data(): total_count(0), courier(0), courier_count(0) {}
	Data(int data): total_count(1), courier(data), courier_count(1) {}
	int total_count;
	int courier;
	int courier_count;

	Data operator+(const Data& other) const {
		return Data(data + other.data);
	}
};

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	SegmentTree<Data> tree(n, InputIterator<int>());

	for (int i = 0; i < m; i++) {
		int a, b;
		std::cin >> a >> b;
		std::cout << tree.sum(a - 1, b).data << '\n';
	}
}

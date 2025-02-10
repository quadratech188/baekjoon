#include "modules/SegmentTree.h"
#include "modules/FastIO.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>

struct Data {
	Data():
		min(0), max(0) {}
	Data(int value):
		min(value), max(value) {}
	Data(int min, int max):
		min(min), max(max) {}

	int min;
	int max;

	void set(int value) {
		min = value;
		max = value;
	}

	Data operator+(const Data& other) const {
		return Data(std::min(min, other.min), std::max(max, other.max));
	}
};

void loop() {
	int n, k;
	std::cin >> n >> k;

	std::vector<int> values(n);
	std::iota(values.begin(), values.end(), 0);
	SegmentTree<Data> tree(values);

	for (int i = 0; i < k; i++) {
		int q, a, b;
		std::cin >> q >> a >> b;

		switch(q) {
			case 0: {
				int last_a = values[a];
				int last_b = values[b];
				std::swap(values[a], values[b]);
				tree.update(a, [last_b](Data& val) {val.set(last_b);});
				tree.update(b, [last_a](Data& val) {val.set(last_a);});
				break;
			}
			case 1: {
				Data result = tree.sum(a, b + 1);
				std::cout << (result.min == a && result.max == b? "YES": "NO") << '\n';
			}
		}
	}
}

int main() {
	FastIO();
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++)
		loop();
}

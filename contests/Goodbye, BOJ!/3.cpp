#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include "SegmentTree2.h"
#include "OverloadedPlus.h"
#include "FastIO.h"
int main() {
	FastIO();
	int n, d;
	std::cin >> n >> d;

	// time, points
	std::vector<std::tuple<int, int, int>> dishes;

	for (int i = 0; i < n; i++) {
		int t, a, b;
		std::cin >> t >> a >> b;
		dishes.emplace_back(t, a, b);
	}

	int score = 0;

	// 1개 만드는 경우
	
	for (auto [t, a, b]: dishes) {
		score = std::max(score, a + b);
	}

	// 2개 만드는 경우
	
	std::sort(dishes.begin(), dishes.end());

	SegmentTree<maxxer<int>> tree(n);
	for (int i = 0; i < n; i++) {
		auto [t, a, b] = dishes[i];
		tree.update(i, [b](maxxer<int>& x) {
				x = b;
				});
	}

	std::vector<int> times(n);
	for (int i = 0; i < n; i++) {
		auto [t, a, b] = dishes[i];
		times[i] = t;
	}

	for (int i = 0; i < n; i++) {
		auto [t, a, b] = dishes[i];
		int time_left = d - t;
		auto ind = std::ranges::upper_bound(times, time_left) - times.begin();
		if (ind == 0) break;

		score = std::max(score, a + tree.sum(0, ind).val());
	}

	std::cout << score << '\n';
}

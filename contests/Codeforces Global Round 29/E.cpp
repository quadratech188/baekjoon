#include "CodeForces.h"
#include "FastIO2.h"
#include "SegmentTree2.h"
#include <iostream>
#include <limits>
#include <queue>
#include <ranges>
/*
 *
 * Problem <=> Add c_1 .. c_n to a_1 .. a_n, sum(a_i) <= b_i
 *
 * a_i = t_31 t_30 ... t_0 (base 2)
 *
 * if (# of enabled k-th bits) >= 2
 * cost = (2's complement up to 2)
 * else
 * cost = 1 << k
 */

struct Element {
	uint index;
	int val;

	Element operator+(Element const& other) const {
		if (val > other.val)
			return *this;
		return other;
	}
};

void loop() {
	uint n, q;
	Fast::cin >> n >> q;

	std::vector<SegmentTree<Element>> minners;
	std::array<int, 31> kth_bit_counts = {0};

	auto values = Fast::cin.to_vec<int>(n);

	for (uint bit = 0; bit < 31; bit++) {
		uint index = 0;
		minners.push_back({values | std::views::transform([&index, &kth_bit_counts, bit](int v) -> Element{
					index ++;
					if (v & (1 << bit))
						kth_bit_counts[bit] ++;
					return {index - 1, v & ((1 << bit) - 1)};
					})});
	}

	for (int query: Fast::cin.to_range<int>(q)) {
		while (true) {
			Element max;
			int cost = std::numeric_limits<int>::max();
			for (uint i = 0; i < 31; i++) {
				Element local_max = minners[i].root();
				int local_cost = (1 << i) - local_max.val;

				for (uint j = 0; j < 31; j++) {
					if ((local_max.val & (1 << j)) == 0) continue;

					if (kth_bit_counts[i] == 1)
						local_cost += (1 << i);
				}

				if (local_cost < cost) {
					cost = local_cost;
					max = local_max;
				}
			}
			if (cost > query) break;
			query -= cost;

			int prev_v = values[max.index];
			values[max.index] += cost;

			for (uint i = 0; i < 31; i++) {
				if (prev_v & (1 << i)) kth_bit_counts[i] --;
				if (values[max.index] & (1 << i)) kth_bit_counts[i] ++;
			}

			for (uint i = 0; i < 31; i++) {
				minners[i].update(max.index, [i, cost](Element e) {
						e.val = (e.val + cost) & (1 << i);
						});
			}
		}

		int cnt = 0;
		for (uint i = 0; i < 31; i++) {
			if (kth_bit_counts[i] != 0) cnt++;
		}
		std::cout << cnt << '\n';
	}
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}

#include "FastIO2.h"
#include "SegmentTree2.h"
#include <algorithm>
#include <bit>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

struct Data {
	uint32_t all;
	uint32_t any;

	Data(uint32_t algorithm_set = 0):
		all(algorithm_set), any(algorithm_set) {}
	Data(uint32_t all, uint32_t any):
		all(all), any(any) {}

	Data operator+(Data const& other) const {
		return {all & other.all, any | other.any};
	}
};
struct Student {
	int skill;
	uint32_t algorithms;

	bool operator<(Student const& other) const {
		return skill < other.skill;
	}

	template <typename IS>
	friend IS& operator>> (IS& is, Student& student) {
		uint m;
		is >> m >> student.skill;
		student.algorithms = 0;
		for (uint i = 0; i < m; i++) {
			uint a;
			is >> a;
			student.algorithms += (1 << a);
		}
		return is;
	}
};

int main() {
	uint n, k;
	int d;
	Fast::cin >> n >> k >> d;

	auto students = Fast::cin.to_vec<Student>(n);

	std::sort(students.begin(), students.end());

	SegmentTree<Data> tree(students | std::views::transform([](Student const& s) {
				return s.algorithms;
				}));
	uint l = 0;
	uint r = 1;

	int max = 0;

	while (true) {
		int skill_diff = students[r - 1].skill - students[l].skill;
		if (skill_diff <= d) {
			Data result = tree.sum(l, r);
			max = std::max(max, (std::popcount(result.any) - std::popcount(result.all)) * int(r - l));
		}

		if (skill_diff <= d || r - l == 1) {
			if (r == n) break;
			r ++;
		}
		else
			l ++;
	}
	std::cout << max;
}

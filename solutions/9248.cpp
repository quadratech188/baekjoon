#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
int main() {
	std::string str;
	std::cin >> str;
	str += '\0';

	std::vector<uint> suffixes(str.size());
	std::iota(suffixes.begin(), suffixes.end(), 0);

	std::vector<uint> equivalence_classes(str.size()), equivalence_classes_back(str.size());

	std::ranges::sort(suffixes, [&str](uint l, uint r) {
			return str[l] < str[r];
			});

	uint counter = 0;
	equivalence_classes[suffixes[0]] = 0;
	for (uint i = 1; i < str.size(); i++) {
		if (str[suffixes[i - 1]] != str[suffixes[i]]) {
			counter ++;
		}
		equivalence_classes[suffixes[i]] = counter;
	}

	for (uint compare_offset = 1; compare_offset < str.size(); compare_offset *= 2) {
		auto to_classes = [&equivalence_classes, compare_offset](uint a) -> std::pair<uint, uint> {
			return {equivalence_classes[a], equivalence_classes[(a + compare_offset) % equivalence_classes.size()]};
		};

		std::ranges::sort(suffixes, [to_classes](uint l, uint r) {
				return to_classes(l) < to_classes(r);
				});

		uint counter = 0;

		// jank
		equivalence_classes_back[suffixes[0]] = 0;
		for (uint i = 1; i < str.size(); i++) {
			if (to_classes(suffixes[i - 1]) != to_classes(suffixes[i])) {
				counter ++;
			}
			equivalence_classes_back[suffixes[i]] = counter;
		}
		std::swap(equivalence_classes_back, equivalence_classes);
	}
	for (uint i = 1; i < str.size(); i++) {
		std::cout << suffixes[i] + 1 << ' ';
	}
}

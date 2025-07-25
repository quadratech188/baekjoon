#include <iostream>
#include <string>
#include <vector>
#include "../modules/Output.h"

int main() {
	std::string S, W;
	std::getline(std::cin, S);
	std::getline(std::cin, W);
	W.push_back('\0');

	std::vector<int> lookup(W.size());

	lookup[0] = -1;

	uint index = 1;
	int prefix_index = 0;
	while (index < W.size() - 1) {
		if (W[prefix_index] == W[index])
			lookup[index] = lookup[prefix_index];
		else {
			lookup[index] = prefix_index;
			while (prefix_index >= 0 && W[index] != W[prefix_index])
				prefix_index = lookup[prefix_index];
		}
		index ++;
		prefix_index ++;
	}
	lookup[index] = prefix_index;

	uint m = 0, i = 0;

	std::vector<uint> matches;

	while (m + i < S.size()) {
		if (S[m + i] == W[i]) {
			i++;
			if (i == W.size() - 1)
				matches.push_back(m);
			// Next iteration - \0 in W causes a mismatch
			continue;
		}

		if (lookup[i] == -1) {
			m += i + 1;
			i = 0;
			continue;
		}

		uint temp = m + i;
		i = lookup[i];
		m = temp - i;
	}
	std::cout << matches.size() << '\n';
	for (auto& match: matches)
		std::cout << match + 1 << ' '; 
}

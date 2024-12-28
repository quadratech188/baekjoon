#include "modules/FastIO.h"
#include "modules/Input.h"
#include "modules/Compress.h"
#include <iostream>

int main() {
	int n;
	std::cin >> n;

	std::vector<int> values = Input::toVec<int>(n);

	Compress<int> compress(values);

	std::vector<bool> used(compress.size());

	int count = 0;

	for (int value: values) {
		int index = compress(value);

		if (used[index]) {
			if ((index > 0 && used[index - 1]) && (index < used.size() - 1 && used[index + 1]))
				count++;
			continue;
		}

		if ((index > 0 && used[index - 1]) || (index < used.size() - 1 && used[index + 1])) {
			used[index] = true;
			continue;
		}

		count++;
		used[index] = true;
	}

	std::cout << count;
}

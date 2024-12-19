#include <iostream>
#include <unordered_map>
#include "modules/Input.h"

bool prime(int p) {
	return true;
}

int solution(std::vector<int>& keys, std::unordered_map<int, int> map, int pointer = 0, int sum = 0) {
	if (pointer == keys.size()) {
		return prime(sum);
	}

	return solution(keys, map, pointer + 1, sum + map[keys[pointer]]) + solution(keys, map, pointer + 1, sum);
}

int main() {
	int n;
	std::cin >> n;

	std::vector<int> costs = Input::inlineToVec<int>(n);

	std::unordered_map<int, int> map;
	std::vector<int> keys;

	for (int cost: costs) {
		if (map[cost] == 0) {
			keys.push_back(cost);
		}
		map[cost] ++;
	}
}

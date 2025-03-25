#include <iostream>
#include <vector>
#include "modules/Input.h"
int main() {
	int l, k, c;
	std::cin >> l >> k >> c;

	std::vector<int> positions = Input::toVec<int>(k);
}

#include <iostream>
#include "../../modules/Input.h"

void loop() {
	auto vec = Input::toVec<char>(10);

	std::sort(vec.begin(), vec.end());
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) loop();
}

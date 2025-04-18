#include "modules/Input.h"
#include "modules/Vec2.h"
#include <iostream>
int main() {
	int c;
	std::cin >> c;

	std::vector<Int2> arrows = Input::toVec<Int2>(c);
}
